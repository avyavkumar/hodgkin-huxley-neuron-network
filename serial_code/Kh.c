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

double Kh(double V, double h)
{
    double r;

    r = ((0.07 * exp(-V / 20)) * (1 - h)) - ((1 / (exp((30 - V) / 10) + 1)) * h);
    return (r);
}
