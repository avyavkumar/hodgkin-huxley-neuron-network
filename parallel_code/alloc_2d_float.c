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

float **alloc_2d_float(int rows, int cols)
{
  int i;
  float *data = (float *)malloc(rows*cols*sizeof(float));
  float **array= (float **)malloc(rows*sizeof(float*));
  for (i=0; i<rows; i++)
      array[i] = &(data[cols*i]);
  return array;
}
