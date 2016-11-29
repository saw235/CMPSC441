#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_SIZE 50
#define NUM_CLIENT 5


int sock_desc, newsockfd, clilen, n;
char buffer[256];
struct sockaddr_in serv_addr, cli_addr;

void * connection(void* threadid)
{


	listen(sock_desc, NUM_CLIENT);

	clilen = sizeof(cli_addr);
	newsockfd = accept(sock_desc, (struct sockaddr *) &cli_addr, &clilen);
	
	if (newsockfd < 0) printf("ERROR on accepting connection.\n");

	bzero(buffer, 256);
	n = read(newsockfd, buffer, 255);

	if ( n < 0) printf("Error reading from socket.\n");

	printf("Here is the message: %s", buffer);

	n = write(newsockfd, "I got your message\n", 19);
	if (n < 0) printf("Error writting to client socket.\n");

	printf("Exiting connection %d\n", (int) threadid);
	pthread_exit(NULL);
}

int main()
{




	if((sock_desc = socket(AF_INET,SOCK_STREAM,0))  < 0 )
		printf("Failed creating socket.\n");

	bzero((char*) &serv_addr, sizeof(serv_addr));
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serv_addr.sin_port = htons(8888);

	if (bind(sock_desc, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) <0)
		printf("ERROR on binding.\n");

	pthread_t thread[2];

	for ( int i = 0;  i < 2; i++)
	{
		pthread_create(&thread[i], NULL, connection, (void *) i);
	}

	for ( int i = 0;  i < 2; i++)
	{
		pthread_join(thread[i], NULL);
	}

	return 0;
}





