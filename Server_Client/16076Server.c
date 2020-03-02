#include <stdio.h>
#include <string.h> 
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
void *server_handler(void *fd_pointer);		//function to recevie message from a client to broadcast it to all
void *server_normally(void *d_pointer);		//function to command all online clients to exit before Server closes normally
static char id_unieque='a';   				//initialize id with base a
static int *user[10];						//store address' of all the user
static int online[10]; 						//keep record of the online user 
static int idx=0; 							//initialize index with 0
char buffer[1000];   						//message block to send to Client
int main(int argc, char *argv[])
{	
	//initialize the user 
	for (int i = 0; i < 10; ++i)
	{
		int *tc; 
		tc=malloc(1);
		*tc=-20;
		user[i]=tc;
		
	}
	int *new_sock;
	int lid=0; 
	int connfd=0;	 
	socklen_t clilen;
	struct sockaddr_in cliaddr, servaddr; 
	lid=socket(AF_INET,SOCK_STREAM,0);      //creates unbound socket
	if (lid==-1)
	{
		perror("Could not create Socket\n"); //throw error if socke is not created		
	}
	printf("Socket Created\n");   
	bzero(&servaddr,sizeof(servaddr)); 		//places zero and initializes the address
	servaddr.sin_family=AF_INET; 
	servaddr.sin_addr.s_addr=INADDR_ANY;
	servaddr.sin_port=htons(8888);			//binds the socket to the port
	if (bind(lid,(struct sockaddr *)& servaddr,sizeof(servaddr))<0)
	{
		/* code */ 
		perror("Unsuccesful Bind");			
		return 1;
	}
	listen(lid,5);							//start listening with window size of 5
	printf("Await Connections\n");
	clilen=sizeof(cliaddr); 

	///handle it
	pthread_t server_quit;
	int *neew_sock=malloc(1);
	neew_sock=malloc(1);  
	*neew_sock=20;
	pthread_create(&server_quit,NULL,server_normally,(void *)neew_sock); //thread created to take input at server side
	while((connfd=accept(lid,(struct sockaddr *)&cliaddr,&clilen)))	     //iterztively accept all those client requesting to accept
	{		
		int givpos=-1;						//find the first empty slot to provide it to the incoming user
 		for (int i = 0; i < 10; i++)
 		{
 			if (online[i]==0)
 			{
 				givpos=i;
 				break;
 			} 
 			else if (i==9)							//check if all the slots are filled
 			{
 				printf("User Maxed Out\n");
 			}
 			 
 		}
 		if (givpos>=0)								//if slot is available
 		{ 		
			idx=givpos; 
			id_unieque='a'; 
			id_unieque=id_unieque+givpos; 			//shift id by distance of slot from base a
			memset(buffer,0,sizeof(buffer));		 
			buffer[0]=id_unieque;	
			printf("Connection of %s Estabilished \n",buffer); 
			send(connfd,buffer,strlen(buffer),0);  //send the incoming client a unieque id
			pthread_t server_thread;
			new_sock=malloc(1);  
			*new_sock=connfd*10+idx;				//add the unique id in the end of user's address to retrieve later
			user[idx] =new_sock;
			online[idx]=1;							//switch the client online
			pthread_create(&server_thread,NULL,server_handler,(void *)new_sock);		//handle broadcasting
			//print the stautus of all the user if online or not
			for (int i = 0; i < 10; i++)
			{		
				int temps=*(int *)user[i]; 
				printf("%d : %d : %d \n",i,online[i],temps);
			}  
			 
		}
	}
	if (connfd<0)
	{ 
		perror("Unable to accept request");			//if unable to accept the client's request
		return 1;
	} 

	return 0;  
}
void *server_normally(void *d_pointer){ 
	while(1) 
	{
		char commnds[1000];
		scanf("%s",commnds);
		if (strlen(commnds)>0)
		{
			char checkto[1000]={'e','x','i','t'}; 
			if (strcmp(commnds,checkto)==0)
			{
				for(int i=0;i<10;i++)
				{  
					if (online[i]==1) 
					{
						int sock2=*(int *)user[i]; 
						sock2=sock2/10; 
						write(sock2,commnds,strlen(commnds));  	//broadcast exit to all the users before quitting and command them to quit too 
					} 			
				}
				printf("Server Disconnected normally\n"); 
				exit(0);    

							////
			}
		}
		memset(commnds, 0, sizeof(commnds));
	}
}
void *server_handler(void *fd_pointer)
{
	int sock=*(int *)fd_pointer;					//store which client sent the message
	int pointo=sock%10;								//retriev clients's id
	sock=sock/10;									//retrieve client's address
	int read_size=0;	
	static char client_message[2000];
	char loc='a';
	loc=loc+pointo;
	int flag=-1;
	while((read_size=recv(sock,client_message,2000,0))>0)		//recieve message sent from the client 
	{
		char buffer2[1000]={'e','x','i','t'};  
		if (strcmp(client_message,buffer2)==0)					//checks if the client sent exit
		{
			flag=1;
			online[pointo]=0;									//puts client offline and empty the slot
			printf("Client Disconnected : %c normally \n",loc ); 			
		}  
		else
		{ 
			for(int i=0;i<10;i++)  								//broadcast message if not exit to all including who sent
			{  
				if (online[i]==1)
				{
					int sock2=*(int *)user[i]; 
					sock2=sock2/10; 
					write(sock2,client_message,strlen(client_message)); 
				} 			
			} 
		}
		memset(client_message, 0, sizeof(client_message));
		//printf("%s\n",client_message ); 
	}  
	 
	if (read_size==0) 								//checks if the client exit abruptly   
	{
		online[pointo]=0;
		if (flag<0)
		{ 
		 	printf("Client Disconnected : %c abruptly\n",loc ); 
		}		 
		
		  
	}   
	else if (read_size==-1)							//if the client goes offline without exit 
	{		
		online[pointo]=0;
		printf("Receive from %c Failed abruptly\n",loc ); 
		
	}
	free(fd_pointer);
	return 0; 
}
  