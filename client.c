#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<time.h>
#include<string.h>
#include<arpa/inet.h>
#include<stdlib.h>

#define CLI_PORT 2100
#define BUFF_SIZE 30

int main(int argc,char **argv)
{
	printf ("Client Starting...");
	int clisoc,re;
	char recbuff[BUFF_SIZE];
	struct sockaddr_in servaddr;
	bzero(&servaddr,sizeof(servaddr));

	servaddr.sin_family=AF_INET;
	servaddr.sin_port=htons(CLI_PORT);
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);

	/* Create a socket */
	if((clisoc=socket(AF_INET,SOCK_STREAM,0))<0)
	{
		perror("Socket Errror");
		exit(0);
	}

	printf ("Socket created, connecting to server\n");
	
	/* Connect to the server process */
	if((connect(clisoc,(struct sockaddr *)&servaddr,sizeof(servaddr)))<0)
	{
		perror("Connect Error");
		exit(0);
	}

	printf("Connected to server, reading from socket\n");
	
	/* Receive the data sent by the server */
	if((re=(read(clisoc,recbuff,sizeof(recbuff))))<0)
	{
		printf("Read Error");
		exit(0);
	}
	
	printf("The Current Date and Time : %s\n",recbuff);
	/* Close the socket and exit */
	close(clisoc);
	return 0;
}
