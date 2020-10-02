#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#include "variables.h"
#include "functions.h"

double KV(double C, double V, double m, double h, double n)
{
    double x;
    int sum = 0;

    x = (((D * C) - ((GNa * xna * (m * m * m) * h * (V - VNa)) + (GK * xk * (n * n * n * n) * (V - VK)) + (GL * (V - VL)))) / c);
    return (x);
}
