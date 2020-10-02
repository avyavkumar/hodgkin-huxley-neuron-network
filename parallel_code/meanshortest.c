#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#include "variables.h"
#include "functions.h"

    //Calculating Mean Shortest Path
    /*You calculate the shortest path between all pairs of nodes using the Floyd-Warshall Algorithm
     * and then use the formula l(G) = (2/n.(n-1))*(sum of all unique shortest path lengths) */

    //Initializing distance matrix with weights
    //All the weights are 1 in our case

double meanshortest(int a[][N], int m)  // This function is identical to the portion earlier for calculating shortest path
{
    int i, j, l = 0;
    int distance[N][N];         //Array containing shortest paths between all pairs of nodes
    double result = 0.00;
    int sumofshortestpaths = 0; //Value used to calculate mean shortest path

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (a[i][j] == 0)
            {
                if (i == j)
                {
                    distance[i][j] = 0;
                    distance[j][i] = 0;
                }
                else
                {
                    distance[i][j] = 100;   //100 is just a very large number. Symbolizes infinity.
                    distance[j][i] = 100;
                }
            }
            else
            {

                distance[j][i] = 1;
                distance[i][j] = 1;
            }
        }
    }

    //Calculating shortest paths
    for (l = 0; l < N; l++)
    {
        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                if (distance[i][j] > (distance[i][l] + distance[l][j]))
                {
                    distance[i][j] = distance[i][l] + distance[l][j];
                }
                else
                    continue;
            }
        }
    }

    //Calculating mean shortest path using the formula l(G) = (2/n.(n-1))*(sum of all unique shortest path lengths)
    for (i = 0; i < N; i++)
    {
        for (j = i + 1; j < N; j++)
        {
            sumofshortestpaths = sumofshortestpaths + distance[i][j];
        }
    }
    result = 2 * (float)sumofshortestpaths / (N * (N - 1));
    return result;
}
