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

double **alloc_2d_double(int rows, int cols)
{
  int i;
  double *data = (double *)malloc(rows*cols*sizeof(double));
  double **array= (double **)malloc(rows*sizeof(double*));
  for (i=0; i<rows; i++)
      array[i] = &(data[cols*i]);
  return array;
}
