#include "variables.h"
#include "functions.h"

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

int **alloc_2d_int(int rows, int cols)
{
  int i;
  int *data = (int *)malloc(rows*cols*sizeof(int));
  int **array= (int **)malloc(rows*sizeof(int*));
  for (i=0; i<rows; i++)
      array[i] = &(data[cols*i]);
  return array;
}
