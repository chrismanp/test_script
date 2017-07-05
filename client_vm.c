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
#include <arpa/inet.h>
#include <sys/time.h>

#define SECOND_2_NANOS 1000000000

#define BACK_LOG 5
#define BUFFER_SIZE 1024

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

  if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
  {
  	perror("socket");
  	exit(EXIT_FAILURE);
  }

  /* Connect to the server */
  memset(&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_port = htons(atoi(argv[2]));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = inet_addr(argv[1]);

  if(connect(sockfd, (const struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0)
  {
  	perror("connect");
  	exit(EXIT_FAILURE);
  }

  int n_iter = 7000;
  int iter = 0;

  struct timeval start_tv;

  memset(myBufferOut, '\0', BUFFER_SIZE);
  memset(myBufferIn, '\0', BUFFER_SIZE);

  for(iter = 0; iter <n_iter; iter++)
  {
  	int n = -1;
    sprintf(myBufferOut, "%d\n", iter);
  	/* Timed here */
  	clock_gettime(CLOCK_MONOTONIC, &startTime);
  	//gettimeofday(&start_tv, NULL);
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
    
    /* Measure the time */
    lStart_t = startTime.tv_sec * SECOND_2_NANOS + startTime.tv_nsec; 
    //lStart_t = start_tv.tv_sec * 1000000 + start_tv.tv_usec;
    lEnd_t = endTime.tv_sec * SECOND_2_NANOS + endTime.tv_nsec; 


    long diff_t = lEnd_t - lStart_t;
  	printf("Receive : %lu - [%d]\n", diff_t, iter);
    /* Write the statistics here */
    fprintf(f_record, "%lu\n", diff_t );
    fflush(f_record);
    usleep(20000);
      
  }

  close(sockfd);
  fclose(f_record);

  exit(EXIT_SUCCESS);
}
