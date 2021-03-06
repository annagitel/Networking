
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
#include <string.h>
#define MAX_MESSAGE_SIZE 1024
#define PORT_CLIENT 1111
#define PORT_ROUTER 3333
#define true 1

int main(int argc, char *argv[])
{
  int socket_fd ; //socket file 
  socklen_t rsize; //socket size
  struct sockaddr_in client, router;  //declairs 2 sockets 
  char message[MAX_MESSAGE_SIZE]; // message sent 
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //init socket fd
 
  /**init inner socket**/
   memset((char *) &client,0, sizeof(client)); 
   client.sin_family = (short)AF_INET; 
   client.sin_addr.s_addr = htonl(INADDR_ANY); 
   client.sin_port = PORT_CLIENT; 

  /**init router socket**/
   router.sin_family = (short)AF_INET; 
   router.sin_addr.s_addr = htonl(INADDR_ANY); 
   router.sin_port = PORT_ROUTER; 
  
  
  bind(socket_fd, (struct sockaddr *)&client, sizeof(client)); //bint socket to port
  
  while(true){
   fflush(stdout); //clear memory
   bzero(message,MAX_MESSAGE_SIZE); //zero all data
   fgets(message,MAX_MESSAGE_SIZE,stdin);  //get message from user
       
   sendto(socket_fd,&message,MAX_MESSAGE_SIZE,0,(struct sockaddr *)&router,sizeof(router)); //send message to router
   rsize = sizeof(router); 
   bzero(message,MAX_MESSAGE_SIZE); //zero all data
   recvfrom(socket_fd, &message, MAX_MESSAGE_SIZE,0,(struct sockaddr *)&router,&rsize); //receve message from router
   printf("Message: %s\n", &message); //print message
   fflush(stdout); //clear data
 }
  close(socket_fd); //close socket
  return 0;
}
