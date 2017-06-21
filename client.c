#include <sys/socket.h>
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

#define SECOND_2_NANOS 1000000000

#define BACK_LOG 5
#define BUFFER_SIZE 1000

int main(int argc, char ** argv)
{
  if(argc != 3)
  {
  	printf("Usage : ./client <Hostname> <Port Number> \n");
  	exit(EXIT_FAILURE);
  }
 
  /* Time parameters */
  struct timespec startTime;
  struct timespec endTime;
  memset(&startTime, 0, sizeof(startTime));
  memset(&endTime, 0, sizeof(endTime));

  long int lStart_t = 0;
  long int lEnd_t = 0;

  /* File parameters */
  FILE * f_record = fopen("server-client-time.log", "w");
  if( f_record == NULL)
  {
    printf("Error in fopen\n");
    exit(EXIT_FAILURE);
  }


  char myBufferIn[BUFFER_SIZE];
  char myBufferOut[BUFFER_SIZE];

  int sockfd = 0;
  struct sockaddr_in serv_addr;
  struct hostent* server;



  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
  	perror("socket");
  	exit(EXIT_FAILURE);
  }

  /* Get the server information */
  server = gethostbyname(argv[1]);
  if(server == NULL)
  {
    printf("gethostbyname failure\n");
    exit(EXIT_FAILURE);
  }

  /* Connect to the server */
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_port = htons(atoi(argv[2]));
  serv_addr.sin_family = AF_INET;

  memcpy((char*) &serv_addr.sin_addr.s_addr, (const char *) server->h_addr, server->h_length);

  if(connect(sockfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0)
  {
  	perror("connect");
  	exit(EXIT_FAILURE);
  }

  int n_iter = 7000;
  int iter = 0;

  memset(myBufferOut, '\0', BUFFER_SIZE);
  memset(myBufferIn, '\0', BUFFER_SIZE);

  for(iter = 0; iter <n_iter; iter++)
  {
  	int n = -1;
    sprintf(myBufferOut, "%d\n", iter);
  	/* Timed here */
  	clock_gettime(CLOCK_MONOTONIC, &startTime);
  	/* Send the message */
  	n = write(sockfd, myBufferOut, BUFFER_SIZE);
  	if(n < 0)
    {
      perror("write");
      exit(EXIT_FAILURE);
    }
  	  
  	/* Receive message */
  	n = read(sockfd, myBufferIn, BUFFER_SIZE);
  	if (n<0)
    {
      perror("read");
      exit(EXIT_FAILURE);
    }
  	  
    /* Timed here */
    clock_gettime(CLOCK_MONOTONIC, &endTime);
    printf("Receive : %s", myBufferIn);

    /* Measure the time */
    lStart_t = startTime.tv_sec * SECOND_2_NANOS + startTime.tv_nsec; 
    lEnd_t = endTime.tv_sec * SECOND_2_NANOS + endTime.tv_nsec; 

    long diff_t = lEnd_t - lStart_t;
  	/* Write the statistics here */
    fprintf(f_record, "%lu\n", diff_t);
    fflush(f_record);
    usleep(20000);
      
  }

  close(sockfd);
  fclose(f_record);

  exit(EXIT_SUCCESS);
}
