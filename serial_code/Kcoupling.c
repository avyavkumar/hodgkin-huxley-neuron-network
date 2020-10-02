#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#include "variables.h"
#include "functions.h"

double Kcoupling(double a[N], double b[][N], int m, double k[N], int index)
{
    //a[N] is the V[j] array at a particular time
    //k[N] is the runge kutta coefficient matrix
    //b[][N] is the edgematrix
    //m is the runge-kutta step flag
    double sum_coupling = 0.0;
    int i = 0;

    for (i = 0; i < N; i++)
    {
        sum_coupling = sum_coupling + (b[index][i] * ((a[i] + 0.5 * k[i]) - (a[index] + 0.5 * k[index])));
    }
    return sum_coupling;
}
