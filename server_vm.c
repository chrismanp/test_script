#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <arpa/inet.h>

#define BACK_LOG 5
#define BUFFER_SIZE 1024

#define SECOND_2_NANOS 1000000000

void spawn_process(int n)
{
  int i = 0;
  char command[100];
  for(i = 0; i<n; i++)
  {
    sprintf(command,"./loop_forever &");
    system(command);
  }
}

int main(int argc, char * argv[])
{
  if(argc != 3)
  {
    printf("Usage ./server <Port Number> <Is busy>\n");
  	exit(EXIT_FAILURE);
  }

  char b_isbusy = atoi(argv[2]);
  if(b_isbusy)
  {
  	/*Spawn process to occupied server */
  	spawn_process(10);
  }

  int listen_sockfd = 0;
  int accept_sockfd = 0;
  unsigned portno = 0;
  int iter = 0;

  /* server address */
  struct sockaddr_in serv_addr;
  memset(&serv_addr, 0, sizeof(struct  sockaddr_in));

  /* client address */
  struct sockaddr_in cl_addr;
  memset(&cl_addr, 0, sizeof(struct sockaddr_in));

  /* Create the fd - socket */
  if((listen_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  /* Bind the address and the specified port number to the server */
  portno = (unsigned) atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);

  if(bind(listen_sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
  {
    perror("bind");
    exit(EXIT_FAILURE);
  }

  /* Listen to a client */
  if(listen (listen_sockfd, BACK_LOG) < 0)
  {
  	perror("listen");
  	exit(EXIT_FAILURE);
  }

  int cl_len = sizeof(cl_addr);
  char myInBuffer[BUFFER_SIZE];
  char myOutBuffer[BUFFER_SIZE];
  int n_readbytes = 0;

  struct timespec endTime;
  memset(&endTime, 0, sizeof(endTime));

  long int lStart_t = 0;
  long int lEnd_t = 0;

  struct timeval start_tv;
  struct timeval end_tv;

  /* Sleep until connected to a client */
  accept_sockfd = accept(listen_sockfd, (struct sockaddr*) &cl_addr, (socklen_t *) &cl_len);

  if(accept_sockfd < 0)
  {
    perror("accept");
    exit(EXIT_FAILURE);
  }

  /* Connection established */
  while(1)
  {
    memset(myOutBuffer, '\0', BUFFER_SIZE);
    memset(myInBuffer, '\0', BUFFER_SIZE);

    gettimeofday(&start_tv, NULL);
    lStart_t = start_tv.tv_sec * 1000000 + start_tv.tv_sec;
    n_readbytes = read(accept_sockfd, myInBuffer, BUFFER_SIZE);
#if 0    
    gettimeofday(&end_tv, NULL);
    lEnd_t = end_tv.tv_sec * 1000000 + end_tv.tv_usec;

    if( (n_readbytes) < 0)
      perror("read");
    else if(n_readbytes == 0)
      break;
    
  
    //lEnd_t = endTime.tv_sec * SECOND_2_NANOS + endTime.tv_nsec; 
    sprintf(myOutBuffer, "%lu\n", lEnd_t - lStart_t);
    printf("Time recieved : %lu [%lu - %lu]\n", lEnd_t - lStart_t, lEnd_t, lStart_t);
#endif 
    /* Write response */
    if(write(accept_sockfd, myInBuffer, BUFFER_SIZE) < 0)
      perror("write");

  }

  close(accept_sockfd);
  close(listen_sockfd);
  
  exit(EXIT_SUCCESS);
}
