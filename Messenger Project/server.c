#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

#define MAX_SIZE 50
#define NUM_CLIENT 2


int n;
int nclient = 0;
int sock_desc, clilen;
int clientsock_list[2];
char shared_buffer[256];
bool event = false;
pthread_mutex_t shared_buffer_mutex;

//bool killserv = false;

struct sockaddr_in serv_addr;

struct connection_thread_data
{
	pthread_t threadid;
	int threadnum;
	char buffer[256];
	int newsockfd;
	struct sockaddr_in cli_addr;
};


void * connection_handler(void* threaddata)
{

	struct connection_thread_data *data = (struct connection_thread_data*) threaddata;
	printf("Connection %d accepted\n", data->threadnum);
	//once accepted connection, waits for message from client 
	while(1)
	{	

		
		bzero(data->buffer, 256);
		n = read(data->newsockfd, data->buffer, 255); //blocking read

		if ( n < 0) printf("Error reading from socket.\n");
		else
		{
			pthread_mutex_lock(&shared_buffer_mutex);
			bzero(shared_buffer, 256);
			event = true;
			printf("User %d : %s\n", data->threadnum ,data->buffer);
		
			if (strcmp(data->buffer, "-1\n") == 0) {
				n = write(data->newsockfd, "-1\n", 3);
				sprintf(shared_buffer, "User %d has left the chat.", data->threadnum);	
				break;
		} 
		/*else if (strcmp(data->buffer, "killserv\n") == 0) {	
			killserv = true;
		}*/
		 	else //triggers event
			{
				sprintf(shared_buffer, "User %d : %s\n", data->threadnum ,data->buffer);			
			}
			pthread_mutex_unlock(&shared_buffer_mutex);
		}

	}

	pthread_mutex_unlock(&shared_buffer_mutex);
	nclient--;
	close(data->newsockfd);
	printf("Exiting connection %d\n", data->threadnum);
	free(data);
	pthread_exit(NULL);
}


void* write_handler(void* threaddata)
{
	while(1){
	pthread_mutex_lock(&shared_buffer_mutex);
		if (event)
		{
			for (int i = 0; i < nclient; i++)
			{
				n = write(clientsock_list[i], shared_buffer, strlen(shared_buffer));
				if (n < 0) printf("Error writting to client socket.\n");
			}
	
			bzero(shared_buffer, 256);
			event = false;
		}
	pthread_mutex_unlock(&shared_buffer_mutex);
	}
}


int main()
{

	pthread_mutex_init(&shared_buffer_mutex, NULL);
	pthread_t write_thread;
	int newsockfd;

	
	if((sock_desc = socket(AF_INET,SOCK_STREAM,0))  < 0 )
		printf("Failed creating socket.\n");

	if (setsockopt(sock_desc, SOL_SOCKET, SO_REUSEADDR, &(int){ 1 }, sizeof(int)) < 0) printf("setsockopt(SO_REUSEADDR) failed");
	bzero((char*) &serv_addr, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(8888);


	//bind address and port to the socket
	if (bind(sock_desc, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0)
		printf("ERROR on binding.\n");


	listen(sock_desc, NUM_CLIENT); //start listening on the server socket end.

	pthread_create(&(write_thread), NULL, write_handler, NULL);
	//int i = 0;
	while(1)
	{

		if (nclient<2)
		{	

		   	struct connection_thread_data* thread_data;

			//allocates memory for thread_data
		   	thread_data = malloc(sizeof(struct connection_thread_data));
		   	newsockfd = accept(sock_desc, (struct sockaddr *) &(thread_data->cli_addr), &clilen); //blocking accept
		

			if (newsockfd < 0){ 
			  printf("ERROR on accepting connection.\n");

			  //if there is an error, there is no need for the thread_data so free it
			  free(thread_data);
			} else { 
			
			//else pass value to the thread_data and use it to create a new thread for handling connections
			thread_data->threadnum = nclient;			
			thread_data->newsockfd = newsockfd;
			
			clientsock_list[nclient] = newsockfd;

			pthread_create(&(thread_data->threadid), NULL, connection_handler, (void *) thread_data); //dostuff

			nclient++;

			}
		}
		
		

	}

	return 0;
}





