
#include <sys/types.h>
#include <unistd.h>    
#include <strings.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SIM_LENGTH 10 
#define IP_ADDRESS "172.20.10.5"
#define PORT 1337 

int main(int argc, char* argv[])
{ 
  int sock; //socket number
  struct sockaddr_in cli_name; //socket struct
  int count;
  int value; 
  struct addrinfo* res;
  char* hostname;
  char* hostaddr;
  struct sockaddr_in* saddr;

  printf("Client is alive and establishing socket connection.\n");
  
  
  sock = socket(AF_INET, SOCK_STREAM, 0); //opening socket
  if (sock < 0)
    { perror ("Error opening channel");
      close(sock); //closes socket and exits program 
      exit(1);
    }
      
  if (argc != 2) { //checks if an argument was given
    perror("Usage: hostnamelookup <hostname>\n");
    exit(1);
  }

  hostname = argv[1]; //sets host name 
  
  if (0 != getaddrinfo(hostname, NULL, NULL, &res)) { //hostname lookup
    fprintf(stderr, "Error in resolving hostname %s\n", hostname);
    exit(1);
  }
  /**init socket**/
  saddr = (struct sockaddr_in*)res->ai_addr;
  hostaddr = inet_ntoa(saddr->sin_addr);

  memset((char *)&cli_name,0, sizeof(cli_name)); 
  cli_name.sin_family = AF_INET; 
  cli_name.sin_addr.s_addr = inet_addr(hostaddr); 
  cli_name.sin_port = htons(PORT);


  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0) //checks connevtion to network
    { perror ("Error establishing communications");
      close(sock); //closes socket and exits program 
      exit(1);
    }


  for (count = 1; count <= SIM_LENGTH; count++)  //recives packets from client
    { read(sock, &value, 4);
      printf("Client has received %d from socket.\n", value);
    }

  printf("Exiting now.\n"); 
//closes socket and exits program 
  close(sock); 
  exit(0); 

} 
