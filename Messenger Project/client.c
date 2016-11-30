#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>

bool b_exit = false;
char rbuffer[256];
char wbuffer[256];
int sockfd;
int n;
void *read_handler(void * threadid)
{
	while(1)
	{
		if (b_exit) {pthread_exit(NULL);}
		
		bzero(rbuffer, 256);
		n = read(sockfd, rbuffer, 255);
		if (n < 0 ) { printf("Error getting messages from server");}
		else { printf("\n%s", rbuffer); }
		
	}	
}
void *write_handler(void * sock)
{

	while(1)
	{
		if (b_exit) {pthread_exit(NULL);}
		printf("\nPlease enter message: ");

	 	bzero(wbuffer, 256);
  		fgets(wbuffer, 255, stdin);


 		n = write(sockfd, wbuffer, strlen(wbuffer));
	}
}

int main()
{

  struct sockaddr_in serv_addr;
  struct hostent *server;

  pthread_t read_thread;
  pthread_t write_thread;

  

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) printf("Error opening socket.\n");

  bzero( (char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8888);

  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) <0)
	{ printf("Error connecting.\n"); return 0;}
  else { printf("Successfully connect to server.\n");}

  pthread_create(&read_thread, NULL, read_handler, NULL);
  pthread_create(&write_thread, NULL, write_handler, (void *) &sockfd);  
while(1)
  {


	if (strcmp(rbuffer, "-1\n") == 0) {b_exit = true; break;}
   }

 return 0;  

}
