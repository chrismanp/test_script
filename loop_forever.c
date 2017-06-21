#include <stdio.h>
#include <stdlib.h>

int main (int argc, char ** argv)
{
  
  int iter = 0;
  float a = 1.2;
  float b = 2.3;
  float c = a;

  while(1)
  {
    c = c * b + iter ;
    c = c * b + iter ;
    c = c * b + iter ;
  }
  exit(EXIT_FAILURE);
}