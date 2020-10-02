#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif

// ADD A PROCESS_NO TO ACCOMODATE THE SCHEDULER PROCESS

int main(int argc, char *argv[])
{
  if (argc != 4)
    {
      fprintf(stderr, "Required arguments are insufficient\nAborting\n");
      return 1;
    }
  else
  {
    int flag;

    float D_min = strtof(argv[1],NULL);
    float D_max = strtof(argv[2],NULL);
    float D_step = strtof(argv[3],NULL);
    float i;
    float D_fraction = 0.00001;

    int total_processes = 0;

    for (i = D_min; i <= D_max + D_fraction; i+= D_step)
      total_processes+=11;

    total_processes++;
    char *bash_script = (char*)malloc(100*sizeof(char));
    char *process = (char*)malloc(10*sizeof(char));

    sprintf(process, "%d", total_processes);

    // strcpy(bash_script, "./construct_rankfile ");
    // strcat(bash_script, process);
    // flag = system(bash_script);
    // if (flag)
    // {
    //   fprintf(stderr, "Failed to launch script %s\n", bash_script);
    //   return 1;
    // }

    strcpy(bash_script, "./construct_directories ");
    strcat(bash_script, argv[1]);
    strcat(bash_script, " ");
    strcat(bash_script, argv[2]);
    strcat(bash_script, " ");
    strcat(bash_script, argv[3]);
    // printf("%s\n", bash_script);
    flag = system(bash_script);
    if (flag)
    {
      fprintf(stderr, "Failed to launch script %s\n", bash_script);
      return 1;
    }

    printf("launching the small-world-network-HH-model...\n");
    strcpy(bash_script, "mpiexec --hostfile hostfile -np ");
    strcat(bash_script, process);
    strcat(bash_script, " ");
    strcat(bash_script, "final_file ");
    strcat(bash_script, argv[1]);
    strcat(bash_script, " ");
    strcat(bash_script, argv[2]);
    strcat(bash_script, " ");
    strcat(bash_script, argv[3]);
    // printf("%s\n", bash_script);
    flag = system(bash_script);
    if (flag)
    {
      fprintf(stderr, "Failed to launch script %s\n", bash_script);
      return 1;
    }

    printf("merging the coupling files...\n");
    strcpy(bash_script, "./merge_files ");
    strcat(bash_script, argv[1]);
    strcat(bash_script, " ");
    strcat(bash_script, argv[2]);
    strcat(bash_script, " ");
    strcat(bash_script, argv[3]);
    flag = system(bash_script);
    if (flag)
    {
      fprintf(stderr, "Failed to launch script %s\n", bash_script);
      return 1;
    }
    free(bash_script);
    free(process);
    printf("deleting temporary files...\n");
    printf("finished\n");
    return 0;
  }
}
