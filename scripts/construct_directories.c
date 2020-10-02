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

int main(int argc, char **argv)
{
  if (argc != 4)
  {
    fprintf(stderr, "Argument mismatch\n Aborting\n");
    return 1;
  }
  else
  {
    int j;
    float D_min = strtof(argv[1],NULL);
    float D_max = strtof(argv[2],NULL);
    float D_step = strtof(argv[3],NULL);
    float D_fraction = 0.00001;
    float i = D_min;

    char *bash_script = (char*)malloc(100*sizeof(char));
    char *base_script = (char*)malloc(100*sizeof(char));
    char *args = (char*)malloc(8*sizeof(char));
    char *trial = (char*)malloc(8*sizeof(char));

    for (i = D_min; i <= D_max + D_fraction; i+= D_step)
    {
      // printf("running for i = %f\n", i);
      strcpy(bash_script, "mkdir ");
      snprintf(args, 8, "%f", i);
      strcat(bash_script, args);
      int flag = system(bash_script);

      if (flag)
      {
        fprintf(stderr, "Aborting\n");
        return 1;
      }

      else
      {
        for (j = 0; j < 10; j++)
        {
          strcat(bash_script, "/");
          strcpy(base_script, bash_script);
          sprintf(trial, "%d", j);
          strcat(bash_script, trial);
          flag = system(bash_script);
          if (flag)
          {
            fprintf(stderr, "Aborting\n");
            return 1;
          }
          strcpy(bash_script, base_script);
        }
      }
    }
    free(bash_script);
    free(args);
    free(trial);
    printf("constructed the directories...\n");
    return 0;
  }
}
