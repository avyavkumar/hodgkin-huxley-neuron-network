#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif

#include "variables.h"
#include "functions.h"

double Kn(double V, double n)
{
    double s;

    s = (0.01 * (10 - V) / (exp((10 - V) / 10) - 1)) * (1 - n) - n * (0.125 * exp(-V / 80));
    return (s);
}
