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


int main(int argc,char* argv[])
{
	int servfd,clientfd;
	char sendbuf[256]="NBA Accrediation",recvbuf[256];
		
	struct sockaddr_in servaddr,clientaddr;
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	servaddr.sin_port = htons(atoi(argv[1]));

	//socket()	
	if((servfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		printf("\nSocket creation failed... Program terminating...");
		exit(1);
	}	
	else
		printf("\nSocket created successfully...");
	
	//bind()
	if(bind(servfd,(struct sockaddr*)&servaddr,sizeof(servaddr)) < 0)
	{
		printf("\nFailed to bind server address into socket descriptor... Program terminating...");
		exit(2);	
	}	
	else
		printf("\nAddress loaded into socket successfully...");
	
	//listen()
	if(listen(servfd,2) < 0)
	{
		printf("\nFailed to listen...Program termianting...");
		exit(3);
	}	
	else		
		printf("\nListening...");
	
	//accept()
	int addrlen = sizeof(servaddr);
	if((clientfd = accept(servfd,(struct sockaddr*)&servaddr,(socklen_t*)&addrlen)) < 0)
	{
		printf("\nFailed to accept connection from client...");	
		exit(4);
	}
	else
		printf("\nSuccessfully connected to client...");
	
	details *info=malloc(sizeof(details));
	info->msglen = strlen(sendbuf);
	info->windowsize=3;
	int ack=0;
	printf("\nMessage length : %d\n",info->msglen);
	
	write(clientfd,info,sizeof(info));
	
	printf("\nMessage to send : %s\n",sendbuf);

	int i,flag,offset=0;
	while(offset < info->msglen)
	{
		flag=-1;
		i=0;
		while(i < info->windowsize)
		{
			if((ack = write(clientfd,&sendbuf[offset],20)) >= 0)
			{
				printf("\nAcknowledgement for Frame %d is %d",i+1,ack);

				printf("\n%c\n",sendbuf[offset]);
				offset++;
			}
			else
			{
				printf("\nAcknowledgement for Frame %d is %d",i+1,ack);	
				flag=i;
			}
			i++;
		}
		if(flag != -1)
			offset=flag;
	}

	printf("\n");
	return 0;
}