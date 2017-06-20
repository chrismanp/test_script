#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pthread.h>

#define SECOND_2_NANOS   1000000000

void* three_macs(void * pn_iter)
{
  int * pn_iter_2 = (int * ) pn_iter;
  int n_iter = * pn_iter_2;

  int iter = 0;
  float a = 1.2;
  float b = 2.3;
  float c = a;

  for(iter = 0; iter < n_iter; iter++)
  {
    c = c * b + iter ;
    c = c * b + iter ;
    c = c * b + iter ;
  }  
}

int main(int argc, char ** argv)
{

  if(argc != 3 && argc != 1)
  {
    printf("Usage : ./loop [number of iteration : default = 500000000] [number of thread : default = 1]\n");
    exit(EXIT_FAILURE);
  }
  int n_iter = 0;
  int n_thread = 0;
  if(argc == 3)
  {
    n_iter = atoi(argv[1]); 
    n_thread = atoi(argv[2]);
  }
  else
  {
    n_iter = 500000000; // Around 9.5-10 second
    n_thread = 1;
  }
  
  /* Time here */
  struct timespec startTime;
  struct timespec endTime;
  memset(&startTime, 0, sizeof(startTime));
  memset(&endTime, 0, sizeof(endTime));

  long int lStart_t = 0;
  long int lEnd_t = 0;

  
  int iter_thread = 0;
  pthread_t* tid = malloc(sizeof(pthread_t) * n_thread);
    
  clock_gettime(CLOCK_MONOTONIC, &startTime);
  for(iter_thread = 0; iter_thread<n_thread; iter_thread++)
  {
      if(pthread_create(&tid[iter_thread], NULL, three_macs, (void*)&n_iter))
      {
        printf("%i thread failure\n",iter_thread);
      }
  }
    
  for(iter_thread = 0; iter_thread<n_thread; iter_thread++)
  {
    pthread_join(tid[iter_thread], NULL);
  }
  clock_gettime(CLOCK_MONOTONIC, &endTime);

  free(tid);  

  /* Measure the time */
  lStart_t = startTime.tv_sec * SECOND_2_NANOS + startTime.tv_nsec; 
  lEnd_t = endTime.tv_sec * SECOND_2_NANOS + endTime.tv_nsec; 
  
  printf("%d,%d,%lf\n", n_iter, n_thread, (float)(lEnd_t - lStart_t)/SECOND_2_NANOS);

  exit(EXIT_SUCCESS);
}

