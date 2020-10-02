#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#include "variables.h"
#include "functions.h"

void* declare_init_files(void* rank)
{
  long my_rank = rank;
  pthread_mutex_t mutex;
  int check = pthread_mutex_init(&mutex, NULL);

  if (my_rank == 0)
  {
    pthread_mutex_lock(&mutex);
    firingcoherenceid = fopen("FiringCoherenceVaryingDs(100,8) (0.9; 0.6).txt", "w");   // This file stores the Firing Coherence values for various coupling strengths (D)
    pthread_mutex_unlock(&mutex);
  }

  if (my_rank == 1)
  {
    pthread_mutex_lock(&mutex);
    fcdegreeid = fopen("FCDegreeVaryingD(100,8) (0.9; 0.6).txt", "w");  // This file stores the Firing Correlation values as a function of degree of separation
    pthread_mutex_unlock(&mutex);
  }

  if (my_rank == 2)
  {
    pthread_mutex_lock(&mutex);
    fisiid = fopen("ISI_no_connections(100,8) (0.9; 0.6).txt", "w");    // This file stores the ISIs of induvidual neurons in the network
    pthread_mutex_unlock(&mutex);
  }

  if(my_rank == 3)
  {
    pthread_mutex_lock(&mutex);
    fmeanisiid = fopen("MeanISI(100,8) (0.9; 0.6).txt", "w");   // This file stores the mean ISI of all the neurons in the network
    pthread_mutex_unlock(&mutex);
  }

  check = pthread_mutex_destroy(&mutex);

  return NULL;
}
