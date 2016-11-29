#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>


int main()
{
  int sockfd, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0) printf("Error opening socket.\n");

  bzero( (char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
  serv_addr.sin_port = htons(8888);

  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) <0)
	{ printf("Error connecting.\n"); return 0;}
  else { printf("Successfully connect to server.\n");}

  while(1)
  {
  	printf("Please enter message: ");

 	 bzero(buffer, 256);
  	fgets(buffer, 255, stdin);

 	 n = write(sockfd, buffer, strlen(buffer));
 
  	bzero(buffer, 256);
  
  	n = read(sockfd, buffer, 255);
	
  	printf("\n%s", buffer);
	
	if (strcmp(buffer, "-1\n") == 0) {break;}
   }

 return 0;  

}
