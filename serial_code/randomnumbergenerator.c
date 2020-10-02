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

/*Function to generate random number between 0 and max_num
 * Random number is generated using current time */

int randomnumbergenerator(int max_num)
{
int result = 0;

result = (rand() % (max_num + 1));
return result;
}
