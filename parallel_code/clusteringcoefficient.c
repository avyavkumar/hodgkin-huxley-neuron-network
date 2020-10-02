#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#include "variables.h"
#include "functions.h"

//Calculating the clustering coefficient.
/*Clustering coefficient is given by:
 * 3*(number of triangles)/(number of triads) */
double clusteringcoefficient(int a[][N], int m)
{
int i, j, l = 0;
int count = 0;
double result = 0.00;       //clustering coefficient to be returned
int no_triads = 0;          //Number of triads
int no_triangles = 0;       //No of triangles

for (i = 0; i < m; i++)
{
    for (j = 0; j < N; j++)
    {
        if (a[i][j] == 1)
        {
            count++;
        }
    }
}

no_triads = count * (count - 1) / 2;
no_triads = count * (count - 1) / 2;
printf("Number of triads = %d\n", no_triads);
for (i = 0; i < N - 2; i++)
{
    for (j = i + 1; j < N - 1; j++)
    {
        for (l = j + 1; l < N; l++)
        {
            if (a[i][j] == 1 && a[j][l] == 1 && a[l][i] == 1)
            {
                no_triangles++;
            }
        }
    }
}
result = (3 * ((float)no_triangles) / ((float)no_triads));
return result;
}
