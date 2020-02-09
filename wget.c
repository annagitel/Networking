#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define PORT 80 
#define MAX_BUFFER_CAP  1024

int main(int argc, char* argv[])
{ 
  struct sockaddr_in cli_name; 
  int sock, bytes_read;
  char hostname[100];
  char path[100];
  char* url;
  char* coefficient="http://";
  struct addrinfo* res;
  struct sockaddr_in* saddr;
  char buffer[MAX_BUFFER_CAP];
  
  if (argc != 2) { //checks if an argument was given
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1);
  }
  
  url = argv[1];


  if (strncmp(url, coefficient, 7) != 0) { //protocol check 
    printf("\n ERR: can't work with https");
    exit(1);
  }

  sscanf(url, "http://%99[^/]/%99[^\n]", hostname, path); //parse host name 
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) { //host name lookup
    fprintf(stderr, "Err: the hostname isn't up %s\n", hostname);
    exit(1);
  }
  
  sock = socket(AF_INET, SOCK_STREAM, 0); //open socket
  if (sock < 0)
    { perror ("Err: can't open socket");
      close(sock);
      exit(1);
    }
	
  printf("Client is alive and establishing socket connection.\n");
  saddr = (struct sockaddr_in*)res->ai_addr;

//init socket
  bzero(&cli_name, sizeof(cli_name)); //zero data
  cli_name.sin_family = AF_INET;  
  cli_name.sin_addr.s_addr = inet_addr(inet_ntoa(saddr->sin_addr)); 
  cli_name.sin_port = htons(PORT);

  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0){ //server connection
    perror ("Error: establishing connection");
    close(sock);
    exit(1);
  }
/**print as requested**/
  printf("GET ");
  printf("%s" , url);
  printf(" HTTP/1.0\n");
  printf("HOST: %s\n", hostname);
  printf("\n");
 
  send(sock, buffer, strlen(buffer), 0); //sending request to the server

//reading data while it comes
  do{
    bzero(buffer, sizeof(buffer)); //zero data
    bytes_read = recv(sock, buffer, sizeof(buffer), 0);
    if ( bytes_read > 0 )
      printf("%s try", buffer);
    }
  while ( bytes_read > 0 );
  printf("Exiting now.\n");

//closes the socket and exits the program
  close(sock); 
  exit(0);

}
