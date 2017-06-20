#include <stdio.h>
#include <stdlib.h>


int main (int argc, char ** argv)
{
    if(argc != 2)
    {
        printf("Usage : ./loop_process number_of_process \n");
        exit(EXIT_FAILURE);
    }
    int iter = 0;
    int n_process = atoi(argv[1]);
    char command[100];

    for (iter = 0; iter <n_process; iter++)
    {
        sprintf(command,"./loop &");
        system(command);
    }

    exit(EXIT_SUCCESS);
}