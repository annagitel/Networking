#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  int socket_fd, cc;
  socklen_t fsize;
  struct sockaddr_in  dest;
  struct hostent *hostptr;
  char* msgbuf; //messege (string)
  char* msgback;

  socket_fd = socket (AF_INET, SOCK_DGRAM, 0); //init socket 
  bzero((char *) &dest, sizeof(dest)); /* They say you must do this */
  hostptr = gethostbyname(argv[1]); //save host name 
  dest.sin_family = (short) AF_INET;
  bcopy(hostptr->h_addr, (char *)&dest.sin_addr,hostptr->h_length);
  dest.sin_port = htons((u_short)0x3333); //set destination port 

  printf("Enter your name: "); 
  scanf("%s", &msgbuf); //gets name from user

  sendto(socket_fd,&msgbuf,sizeof(msgbuf),0,(struct sockaddr *)&dest,sizeof(dest)); //sends packet to server

  for(;;) { //infinit loop that listens to the socket
    fsize = sizeof(dest);
    cc=recvfrom(socket_fd,&msgback,sizeof(msgback),0,(struct sockaddr *)&dest,&fsize); //receves packet from sender
    printf("Got data back: %s\n", &msgback); //prints the messege
    fflush(stdout);
  }

  return 0;
}

