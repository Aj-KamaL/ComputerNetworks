#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>    
#include <unistd.h> 
#include <pthread.h>
void *client_handler(void *fd_pointer);		//function to handle client sending message to server
char this_id='a';
int main(int argc, char *argv[])
{ 
	int sockfd=0; 
	char server_reply[2000]; 
	struct sockaddr_in servaddr;
	int *pass_sock;
	sockfd=socket(AF_INET,SOCK_STREAM,0); 
	if(sockfd==-1){ 
		perror("Unable to Create Socket");  
	}  
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family=AF_INET;      
	servaddr.sin_addr.s_addr=inet_addr(argv[1]);  		//takes the passed argument of Server's IP
	servaddr.sin_port=htons(8888);

	connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));			//connects to the server

	memset(server_reply, 0, sizeof(server_reply));
	if (recv(sockfd,server_reply,2000,0)<0) 
	{  
		puts("Error");     
		return 0;
	}  
	this_id=server_reply[0]; 
	printf("Client ID : %s \n",server_reply ); 
	pthread_t client_thread; 
	pass_sock=malloc(1); 
	*pass_sock=sockfd;	
	pthread_create(&client_thread,NULL,client_handler,(void *)pass_sock);   
	while(1)																//client continuously listens for broadcast
	{  
		memset(server_reply, 0, sizeof(server_reply));	
														/* added disc to check if the server is up, if not client must terminate */	 
		int disc=recv(sockfd,server_reply,2000,0);

		if (disc<=0) 
		{  
			printf("Server Disconnected Abruptly\n");
			return 0;
		}		  
		else if (strlen(server_reply)!=0)
		{
			char buffer2[1000]={'e','x','i','t'};
			if(strcmp(server_reply,buffer2)==0)			/* checks if server send exit, client must terminate */
			{
				printf("Server Disconnected normally\n");
				exit(0);
			} 
			int t_len2=strlen(server_reply);
		    char tempp=server_reply[t_len2-1];
		    server_reply[t_len2-1]='\0';
			printf("%c : %s \n",tempp,server_reply );	 
		}		 
	}  
	close(sockfd);     
	return 0;    
}    
void *client_handler(void *fd_pointer){
	int sockd=*(int *)fd_pointer;
	while(1){		
		char buffer[1000];
		scanf("%s",buffer);
		char buffer2[1000]={'e','x','i','t'};
		if (strlen(buffer)!=0) 									
		{
			if(strcmp(buffer,buffer2)==0){							//if clients want to exit, notify user before exit   
				memset(buffer,0,sizeof(buffer));
				strcat(buffer,"exit"); 
				int disc=send(sockd,buffer,strlen(buffer),0);
				if (disc<=0)
				{   
					printf("Server Disconnected Abruptly\n"); 
					return 0;
				}
				printf("Client Disconnected normally\n");
				exit(0);
			}  
			else 														  		//else send it to the server 
			{  
				int t_len2=strlen(buffer);		       
			    buffer[t_len2]=this_id;  
			    buffer[t_len2+1]='\0'; 
														/* added disc to check if the server is up, if not client must terminate */
				int disc=send(sockd,buffer,strlen(buffer),0);
				if (disc<=0)
				{   
					printf("Server Disconnected Abruptly\n"); 
					return 0;
				}
			}
		} 
		memset(buffer,0,sizeof(buffer)); 
	}
} 
