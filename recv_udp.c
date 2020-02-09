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
 

void printsin(struct sockaddr_in *sin, char *pname, char *msg) {
  printf("%s\n", pname);
  printf("%s: ", msg);
  int port = sin->sin_port;  
  struct in_addr IP = sin->sin_addr;
  printf("ip=%s ,", inet_ntoa(IP));
  printf("port=%d ", port);
  printf("\n");
}

 
int main(int argc, char *argv[])
{
  int socket_fd;
  socklen_t fsize;
  struct sockaddr_in  s_in, from;
  char* msg;
 
  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //init socket 
 
  bzero((char *) &s_in, sizeof(s_in));  //
 
  s_in.sin_family = (short)AF_INET;
  s_in.sin_addr.s_addr = htonl(INADDR_ANY);    /* WILDCARD */
  s_in.sin_port = htons((u_short)0x3333); //init socket port
 
  printsin(&s_in, "UDP-SERVER:", "Local socket is:"); //print servers details
  fflush(stdout); //flush 
 
  bind(socket_fd, (struct sockaddr *)&s_in, sizeof(s_in)); //Establish connection to port
 
  for(;;) { //infinit loop that listens to the socket
    fsize = sizeof(from);
    recvfrom(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,&fsize); //receves packet from sender
    printsin((struct sockaddr_in*)&from, "UDP-SERVER: ", "Packet from:"); //prints details of sender
    printf("Got data: %s\n", &msg); //prints the messege
	//added this:
    sendto(socket_fd,&msg,sizeof(msg),0,(struct sockaddr *)&from,
                  sizeof(from)); //sends packet to server
    fflush(stdout);
  }
   
  return 0;
}
