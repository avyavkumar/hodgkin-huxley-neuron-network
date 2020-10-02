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

void* declare_trial_files(void* rank)
{
  long my_rank = rank;
  pthread_mutex_t mutex;
  int check = pthread_mutex_init(&mutex, NULL);

  if (my_rank == 0)
  {
    pthread_mutex_lock(&mutex);
    fV_vs_t_id = fopen("V_vs_T.txt", "w");
    pthread_mutex_unlock(&mutex);
  }

  if (my_rank == 1)
  {
    pthread_mutex_lock(&mutex);
    fspiketimeid = fopen("Spiketime.txt", "w");
    pthread_mutex_unlock(&mutex);
  }

  if (my_rank == 2)
  {
    pthread_mutex_lock(&mutex);
    yid = fopen("Y.txt", "w");
    pthread_mutex_unlock(&mutex);
  }

  if(my_rank == 3)
  {
    pthread_mutex_lock(&mutex);
    fVTid = fopen("VT.txt", "w");
    pthread_mutex_unlock(&mutex);
  }

  if (my_rank == 4)
  {
    pthread_mutex_lock(&mutex);
    fAdjacencymatrixId = fopen("RewiredMatrix.txt", "w");
    pthread_mutex_unlock(&mutex);
  }

  if (my_rank == 5)
  {
    pthread_mutex_lock(&mutex);
    frandomnumberId = fopen("Randomnumbers.txt", "w");
    pthread_mutex_unlock(&mutex);
  }

  if (my_rank == 6)
  {
    pthread_mutex_lock(&mutex);
    fNoConnectionsId = fopen("NoOfConnections.txt", "w");
    pthread_mutex_unlock(&mutex);
  }

  if(my_rank == 7)
  {
    pthread_mutex_lock(&mutex);
    fm_noiseId = fopen("m_noise.txt", "w");
    pthread_mutex_unlock(&mutex);
  }

  if (my_rank == 8)
  {
    pthread_mutex_lock(&mutex);
    fmvalueId = fopen("mvalues.txt", "w");
    pthread_mutex_unlock(&mutex);
  }

  if (my_rank == 9)
  {
    pthread_mutex_lock(&mutex);
    fhvalueId = fopen("hvalues.txt", "w");
    pthread_mutex_unlock(&mutex);
  }

  if (my_rank == 10)
  {
    pthread_mutex_lock(&mutex);
    fnvalueId = fopen("nvalues.txt", "w");
    pthread_mutex_unlock(&mutex);
  }

  check = pthread_mutex_destroy(&mutex);

  return NULL;
}
