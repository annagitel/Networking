
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> 
#define PORT_CLIENT 1111 
#define PORT_SERVER 2222 
#define PORT_ROUTER 3333 
#define MAX_MESSAGE_SIZE 1024 
double MakeRandom(int,int);
double MakeRandom(int min, int max) //this function generates a random number
{
    double b;
    float r;
    srand(time(NULL));
    for (b=11;b>0;b--){
        r = rand();
       return (r/RAND_MAX);
    }
  return 0;
}

int main(int argc, char *argv[])
{
    int socket_fd; //socket file 
    socklen_t fsize; //socket size
    struct sockaddr_in  router, from, client, server; //declare 4 sockets
    char message[MAX_MESSAGE_SIZE]; 
    double num, random; 
    printf("Enter Number: \n"); 
    scanf("%lf",&num);
    random = MakeRandom(0,1);
	  
      
      socket_fd = socket (AF_INET, SOCK_DGRAM, 0); 
       memset((char* ) &router,0, sizeof(router));  
       memset((char *) &client,0, sizeof(client));
       memset((char *) &server,0, sizeof(server));

       /**init inner socket**/
       router.sin_family = (short)AF_INET; 
       router.sin_addr.s_addr = htonl(INADDR_ANY);    
       router.sin_port = PORT_ROUTER; 

       /**init client socket**/
       client.sin_family = (short)AF_INET;  
       client.sin_addr.s_addr = htonl(INADDR_ANY);  
       client.sin_port = PORT_CLIENT; 

       /**init server socket**/
       server.sin_family = (short)AF_INET;  
       server.sin_addr.s_addr = htonl(INADDR_ANY);  
       server.sin_port = PORT_SERVER; 
     
       
       bind(socket_fd, (struct sockaddr *)&router, sizeof(router)); //bind socket to port
    
      while(1){ //inifinit loop 
         fsize = sizeof(from);
         bzero(message,MAX_MESSAGE_SIZE); //zero all data
         recvfrom(socket_fd, &message, MAX_MESSAGE_SIZE,0,(struct sockaddr *)&from,&fsize); //recive message from client/server
         printf("Message: %s\n", message);
         fflush(stdout);
	 if(from.sin_port == PORT_CLIENT) //who recived from
 	{ 
		if(random > num ) {
			puts("Redirecting message\n");
			sendto(socket_fd,&message,MAX_MESSAGE_SIZE,0,(struct sockaddr *)&server,sizeof(server)); //send to server
		}
		else {  
			puts("Message Dropped\n");
			strcpy(message,"Message Lost\n");
			sendto(socket_fd,&message,MAX_MESSAGE_SIZE,0,(struct sockaddr *)&client,sizeof(client));
		}
	 }
         else{ //if message from server- just sends it to client 
              printf("Redirecting message\n");
              sendto(socket_fd,&message,MAX_MESSAGE_SIZE,0,(struct sockaddr *)&client,sizeof(client)); //sends to client 
         }
      }
    return 0;
}

