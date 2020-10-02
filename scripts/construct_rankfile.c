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

// MODIFY THE RANKFILE TO ENSURE THAT RANK 0 IS MAPPED TO ONLY ONE CORE

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    fprintf(stderr, "Parameter mismatch\nAborting\n");
    return 1;
  }
  else
  {
    int processors, i, flag, rank_id;
    char *temp;
    int processes = strtol(argv[1], &temp, 10);

    flag = system("nproc > processors.txt ");
    if (flag)
    {
      fprintf(stderr, "Failed to launch script nproc > processors.txt\n");
      return 1;
    }

    FILE *no_processors = fopen("processors.txt", "r");
    fscanf(no_processors, "%d", &processors);
    fclose(no_processors);

    flag = system("rm processors.txt");
    if (flag)
    {
      fprintf(stderr, "Failed to launch script rm processors.txt\n");
      return 1;
    }

    FILE *rankfile = fopen("rankfile", "w");
    fprintf(rankfile, "rank 0=localhost slot=0:0\n");

    for (i = 1; i < processes; i++)
    {
      rank_id = 1;
      while (rank_id != processors)
        {
          fprintf(rankfile, "rank %d=bits-goa slot=0:%d\n", i, rank_id);
          rank_id++;
          i++;
        }
      i--;
    }
    printf("constructed the rankfile...\n");
    return 0;
  }
}
