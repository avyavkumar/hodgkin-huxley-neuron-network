#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#include "variables.h"
#include "functions.h"

double Kcoupling1(double a[N], double b[][N], int m, double k[N], int index)
{
    //a[N] is the V[j] array at a particular time
    //k[N] is the runge kutta coefficient matrix
    //b[][N] is the edgematrix
    //m is the runge-kutta step flag
    double sum_coupling = 0.0;
    int i = 0;

    for (i = 0; i < N; i++)
    {
        sum_coupling = sum_coupling + (b[index][i] * ((a[i] + k[i]) - (a[index] + k[index])));
    }
    return sum_coupling;
}
