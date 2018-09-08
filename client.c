#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<unistd.h>

typedef struct Details
{
	int windowsize;
	int msglen;
}details;


int main(int argc,char *argv[])
{
	int clientfd;
	char recvbuf[256],sendbuf[256];
	
	struct sockaddr_in clientaddr;
	clientaddr.sin_family = AF_INET;
	clientaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	clientaddr.sin_port = htons(atoi(argv[1]));

	//socket()	
	if((clientfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		printf("\nSocket creation failed... Program terminating...");
		exit(1);
	}	
	else
		printf("\nSocket created successfully...");
		
	//accept()
	if(connect(clientfd,(struct sockaddr*)&clientaddr,sizeof(clientaddr)) < 0)
	{
		printf("\nFailed to connect server...");
		exit(1);	
	}
	else
		printf("\nSuccessfully connected to server...");
	

	details *info=malloc(sizeof(details));
	
	read(clientfd,info,sizeof(info));
	perror("Window Size");
	printf("\nExpected Message length : %d\n",info->msglen);
	printf("\nAccepted Window size : %d\n",info->windowsize);

	printf("\nFD : %d\n",clientfd);

	int i,offset=0;
	while(offset <= info->msglen)
	{
		i=0;
		while(i < info->windowsize)
		{
			read(clientfd,&recvbuf[offset],20);
			printf("%c",recvbuf[offset]);
			offset++;

			i++;
		}
	}
	recvbuf[offset++]='\0';
	printf("\nMessage received : %s\n",recvbuf);

	return 0;
}