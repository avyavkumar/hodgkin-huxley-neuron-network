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

double Km(double V, double m)
{
    double q;

    q = ((((0.1 * (25 - V)) / (exp((25 - V) / 10) - 1)) * (1 - m)) - (m * (4 * exp(-V / 18))));
    return (q);
}
