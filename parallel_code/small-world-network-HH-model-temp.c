/*
This code generates a small world network with N nodes with k average connections per node.
It then simulates N stochastic HH neurons connected in such a topology using the 4th order Runge - Kutta method for t_max seconds.
It calculates Mean ISI, Firing Correlation, Avg Firing correlation etc.
*/

#include "functions.h"
#include "variables.h"

#ifndef TIME_H
#define TIME_H
#include <time.h>
#endif

#ifndef PTHREAD_H
#define PTHREAD_H
#include <pthread.h>
#endif

#ifndef STDIO_H
#define STDIO_H
#include <stdio.h>
#endif

#ifndef MPI_H
#define MPI_H
#include <mpi.h>
#endif

#ifndef STRING_H
#define STRING_H
#include <string.h>
#endif

#ifndef MATH_H
#define MATH_H
#include <math.h>
#endif

#ifndef STDLIB_H
#define STDLIB_H
#include <stdlib.h>
#endif

int main(int argc, char **argv)
{
  int world_size;
  float dvalue = 0.0;
  int flag_script;
  int recvd_proc_rank;
  int send_to_rank;
  int procs_completed = 1;
  int my_rank;
  int loop_var;
  int processors;
  int n_steps = (int)time_max / dt;
  char processor_name[MPI_MAX_PROCESSOR_NAME];
  int name_length;
  int current_trial;
  int base_rank = 1;
  int loop_ends = 0;
  char *file_path = (char*)malloc(100*sizeof(char));
  char *base_path = (char*)malloc(100*sizeof(char));
  char *coupling_path = (char*)malloc(8*sizeof(char));
  char *trial_path = (char*)malloc(8*sizeof(char));
  double _FinalISI[10][100];
  // _FinalISI = alloc_2d_float(10,100);
  int _no_spikes[10][100];
  // _no_spikes = alloc_2d_int(10,100);
  double _connections[10][100];
  // _connections = alloc_2d_double(10,100);
  double _tot_degree[10][12];
  // _tot_degree = alloc_2d_double(10,12);
  int _count_degree[10][12];
  // _count_degree = alloc_2d_int(10,12);
  double *_firing_coherence = (double *)malloc(10*sizeof(double));
  int *recvd_rank = (int *)malloc(10*sizeof(int));
  int rec_proc_rank;

  int i, j, l;            //Looping variables. Not using k because it is already used

  MPI_Init(&argc, &argv);
  float dmin = strtof(argv[1], NULL);
  float dmax = strtof(argv[2], NULL);
  float dstep = strtof(argv[3], NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
  MPI_Get_processor_name(processor_name, &name_length);

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////

  // the scheduler process is mapped to the the first core of the first socket

  if (my_rank == 0)
  {
    // printf("value of N is %d", N);
    srand(time(NULL) + my_rank*world_size);
    flag_script = system("nproc > processors.txt ");
    if (flag_script)
    {
      fprintf(stderr, "Failed to launch script nproc > processors.txt\n");
      return 1;
    }

    FILE *no_processors = fopen("processors.txt", "r");
    fscanf(no_processors, "%d", &processors);
    fclose(no_processors);

    flag_script = system("rm processors.txt");
    if (flag_script)
    {
      fprintf(stderr, "Failed to launch script rm processors.txt\n");
      return 1;
    }

    ///////////////////////////////check the condition////////////////////////////////////////////////////////////

    for(loop_var = 0; loop_var < (3*processors)/11; loop_var++)
    {
      //using a synchronous send to different cores which correspond to coupling strength values
      // in case of processors > processes, keep a check condition based on dmax
      if (base_rank < world_size)
      {
        dvalue = dmin + loop_var*dstep;
        MPI_Send(&dvalue, 1, MPI_FLOAT, base_rank, 0, MPI_COMM_WORLD);
        base_rank+=11;
        // procs_completed+=10;
      }
    }

    while (1)
    {
      // Receive the rank of the process completed

      MPI_Recv(&recvd_proc_rank,1,MPI_INT,MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      // printf("RECEIVED PROCESS %d\n", recvd_proc_rank);
      if ((recvd_proc_rank-1)%11 == 0)    // Safeguard to ensure that only the coupling process sends back data
      {
        procs_completed+=11; // or 11? Increment the number of completed
        // printf("procs completed are %d\n",procs_completed);
        if (procs_completed == world_size)
        {
            break;
        }

        if (base_rank < world_size)  // Send the computed process to the free set of cores
        {
          // instead of changing the rankfile, find the process(es) which yields the same mapping to the cores
          // as the coupling_strength process which completed, and launch it
          dvalue = dmin + dstep*(base_rank/11);
          // printf("Sending the dvalue %f with dmax as %f to %d\n", dvalue, dmax, base_rank);
          MPI_Send(&dvalue, 1, MPI_FLOAT, base_rank, 0, MPI_COMM_WORLD);
          // printf("SENT PROCESS %d\n", recvd_proc_rank);
          base_rank+=11;
        }
      }
      if ((recvd_proc_rank-1)%11 != 0 )
      {
        printf("ERROR ENCOUNTERED\n");
      }
    }
  }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // First thread section, associate each file pointer with a thread for initialisation
    // Destroy threads and dynamic memory after allocation

    // long thread = 0;
    // int thread_count = 4;
    // pthread_t *thread_handles;
    // pthread_attr_t attr;
    //
    // thread_handles = malloc(thread_count*sizeof(pthread_t));
    // pthread_attr_init(&attr);
    //
    // pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
    //
    // for (thread = 0; thread < thread_count; thread++)
    // {
    //   pthread_create(&thread_handles[thread], &attr, declare_init_files, (void*) thread);
    // }
    //
    // for(thread = 0; thread < thread_count; thread++)
    // {
    //   pthread_join(thread_handles[thread], NULL);
    // }
    //
    // free(thread_handles);

    //End of first thread section


  ///////////////////////////////////////////////////////////////////////////////////////////////////////////

  // TO DO
  // Compute a stringbuilder for the file paths
  // Recieve all the variables and do the computations
  // Write the values to the files
  // Send the final computations to the coupling process

  if ((my_rank-1)%11 == 0)
  {
    // printf("Rank is %d", my_rank);
    srand(time(NULL) + my_rank*world_size);
    // Recieve the coupling strength value from the scheduler process;

    MPI_Recv(&dvalue, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    snprintf(coupling_path, 8, "%f", dvalue);
    getcwd(file_path, 100);
    strcat(file_path, "/");
    strcat(file_path,coupling_path);
    strcat(file_path, "/");
    strcpy(base_path, file_path);

    // This file stores the Firing Coherence values for various coupling strengths (D)

    strcat(file_path, "FiringCoherenceVaryingDs(100,8)(0.9;0.6).txt");
    firingcoherenceid = fopen(file_path, "w");

    // This file stores the Firing Correlation values as a function of degree of separation

    strcpy(file_path, base_path);
    strcat(file_path, "FCDegreeVaryingD(100,8)(0.9;0.6).txt");
    fcdegreeid = fopen(file_path, "w");

    // This file stores the ISIs of induvidual neurons in the network

    strcpy(file_path, base_path);
    strcat(file_path, "ISI_no_connections(100,8)(0.9;0.6).txt");
    fisiid = fopen(file_path, "w");

    // This file stores the mean ISI of all the neurons in the network

    strcpy(file_path, base_path);
    strcat(file_path, "MeanISI(100,8)(0.9;0.6).txt");
    fmeanisiid = fopen(file_path, "w");

    // printf("D = %f\n", dvalue);

    /*We are calculating the firing correlation as a function of degree in the network
     * We are averaging our results over a number of variations in the network to reduce
     * the effects of noise on the validity of our results.
     * tot_degree is a variable which stores the firing correlation value over a particular degree of separation
     * Over 'n' trials, we add up the values of firing correlation for one particular degree of separation in this
     * and divide by the number of trials in which neurons are separated by those many number of hops
     * count_degree stores the number of trials in which there exists a particular degree of separation
     */

    for (i = 0; i < N; i++) // i ---> number of neurons
    {
      no_spikes[i] = 0;
      FinalISI[i] = 0.0;
      connections[i] = 0;
    }

    for (i = 0; i < 12; i++)    // i ---> degree of separation
    {
      tot_degree[i] = 0;
      count_degree[i] = 0;
    }

    // To send -
    // Trial number to each core
    // FinalISI over all the trials
    // no_spikes over all the trials
    // connections over all the trials
    // count_degree over all the trials
    // tot_degree over all the trials
    // firing_coherence over all the trials

    // trial / current_trial
    for (send_to_rank = my_rank + 1, trial = 0; send_to_rank < my_rank + 11; send_to_rank++, trial++)
    {
      MPI_Send(&(FinalISI[0]), 100, MPI_DOUBLE, send_to_rank, 0, MPI_COMM_WORLD);
      MPI_Send(&(no_spikes[0]), 100, MPI_INT, send_to_rank, 1, MPI_COMM_WORLD);
      MPI_Send(&(connections[0]), 100, MPI_DOUBLE, send_to_rank, 2, MPI_COMM_WORLD);
      MPI_Send(&tot_degree[0], 12, MPI_DOUBLE, send_to_rank, 3, MPI_COMM_WORLD);
      MPI_Send(&count_degree[0], 12, MPI_INT, send_to_rank, 4, MPI_COMM_WORLD);
      MPI_Send(&firing_coherence, 1, MPI_DOUBLE, send_to_rank, 5, MPI_COMM_WORLD);
      MPI_Send(&my_rank, 1, MPI_INT, send_to_rank, 6, MPI_COMM_WORLD);
      MPI_Send(&dvalue, 1, MPI_FLOAT, send_to_rank, 7, MPI_COMM_WORLD);
      MPI_Send(&trial, 1, MPI_INT, send_to_rank, 8, MPI_COMM_WORLD);
    }

    // To recieve -
    // FinalISI over all the trials
    // no_spikes over all the trials
    // connections over all the trials
    // count_degree over all the trials
    // tot_degree over all the trials
    // firing_coherence over all the trials

    for (i = 0; i < N; i++) // i ---> number of neurons
    {
      no_spikes[i] = 0;
      FinalISI[i] = 0.0;
      connections[i] = 0;
    }

    for (i = 0; i < 12; i++)    // i ---> degree of separation
    {
      tot_degree[i] = 0;
      count_degree[i] = 0;
    }

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      for (i = 0; i < 100; i++)
      {
        _FinalISI[loop_var][i] = 0;
      }
    }

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      for (i = 0; i < 100; i++)
      {
        _no_spikes[loop_var][i] = 0;
      }
    }

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      for (i = 0; i < 100; i++)
      {
        _connections[loop_var][i] = 0;
      }
    }

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      for (i = 0; i < 12; i++)
      {
        _tot_degree[loop_var][i] = 0;
      }
    }

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      for (i = 0; i < 12; i++)
      {
        _count_degree[loop_var][i] = 0;
      }
    }

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      _firing_coherence[loop_var] = 0;
    }

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      recvd_rank[loop_var] = 0;
    }

    for (send_to_rank = my_rank + 1, loop_var = 0; send_to_rank < my_rank + 11; send_to_rank++, loop_var++)
    {
      // MPI_Send(&(FinalISI[0]), 100, MPI_DOUBLE, recvd_proc_rank, 0, MPI_COMM_WORLD);
      // MPI_Send(&(no_spikes[0]), 100, MPI_INT, recvd_proc_rank, 1, MPI_COMM_WORLD);
      // MPI_Send(&(connections[0]), 100, MPI_DOUBLE, recvd_proc_rank, 2, MPI_COMM_WORLD);
      // MPI_Send(&tot_degree[0], 12, MPI_DOUBLE, recvd_proc_rank, 3, MPI_COMM_WORLD);
      // MPI_Send(&count_degree[0], 12, MPI_INT, recvd_proc_rank, 4, MPI_COMM_WORLD);
      // MPI_Send(&firing_coherence, 1, MPI_DOUBLE, recvd_proc_rank, 5, MPI_COMM_WORLD);
      // MPI_Send(&recvd_rank, 1, MPI_INT, recvd_proc_rank, 6, MPI_COMM_WORLD);

      MPI_Recv(&(_FinalISI[loop_var][0]), 100, MPI_DOUBLE, send_to_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&(_no_spikes[loop_var][0]), 100, MPI_INT, send_to_rank, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&(_connections[loop_var][0]), 100, MPI_DOUBLE, send_to_rank, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&_tot_degree[loop_var][0], 12, MPI_DOUBLE, send_to_rank, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&_count_degree[loop_var][0], 12, MPI_INT, send_to_rank, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&_firing_coherence[loop_var], 1, MPI_DOUBLE, send_to_rank, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      MPI_Recv(&recvd_rank[loop_var], 1, MPI_INT, send_to_rank, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      if (isnan(_firing_coherence[loop_var]) == 0)
        firing_coherence += _firing_coherence[loop_var];
    }

    // for (loop_var = 0; loop_var < 10; loop_var++)
    // {
    //   for (i = 0; i < 100; i++)
    //   {
    //     printf("%f ",_connections[loop_var][i]);
    //   }
    //   printf("\n");
    // }

    firing_coherence/=10.0;

    // printf("Firing Coherence = %f\n", firing_coherence);
    fprintf(firingcoherenceid, "%f\t%f\n", dvalue, firing_coherence);

    fprintf(fcdegreeid, "#D = %f\n", dvalue);
    fprintf(fcdegreeid, "\n");

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      for (i = 0; i < 12; i++)
      {
        if (isnan(_tot_degree[loop_var][i]) == 0)
          tot_degree[i]+=_tot_degree[loop_var][i];
      }
    }

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      for (i = 0; i < 12; i++)
      {
        if (isnan(_count_degree[loop_var][i]) == 0)
          count_degree[i]+=_count_degree[loop_var][i];
      }
    }

    for (i = 0; i < 12; i++)
    {
      tot_degree[i] = tot_degree[i] / count_degree[i];
      //fprintf(degreefcid, "%d\t%f\n",i+1,tot_degree[i]);
      fprintf(fcdegreeid, "%d\t%f\n", i + 1, tot_degree[i]);
    }

    fprintf(fcdegreeid, "\n");
    fprintf(fisiid, "#D = %f\n", dvalue);
    double mean = 0.0;
    double mean1 = 0.0;
    double stddev = 0.0;

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      for (i = 0; i < N; i++)
      {
        if (isnan(_FinalISI[loop_var][i]) == 0)
          FinalISI[i]+=_FinalISI[loop_var][i];
      }
    }

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      for (i = 0; i < N; i++)
      {
        if (isnan(_connections[loop_var][i]) == 0)
          connections[i]+=_connections[loop_var][i];
      }
    }

    for (loop_var = 0; loop_var < 10; loop_var++)
    {
      for (i = 0; i < N; i++)
      {
        if (isnan(_no_spikes[loop_var][i]) == 0)
          no_spikes[i]+=_no_spikes[loop_var][i];
      }
    }

    for (i = 0; i < N; i++)
    {
      FinalISI[i] = FinalISI[i] / 10.0;
      mean = mean + FinalISI[i];
      mean1 = mean1 + (FinalISI[i] * FinalISI[i]);
      //connections[i] = connections[i]/2.0;
      // FinalISI, connections averaged over all trials.
      fprintf(fisiid, "%d\t %f\t %d\t %f\n", i + 1, connections[i], no_spikes[i], FinalISI[i]);
    }

    fprintf(fisiid, "\n");
    mean = mean / N;
    mean1 = mean1 / N;
    stddev = sqrt(mean1 - (mean * mean));
    fprintf(fmeanisiid, "#D = %f\n", dvalue);
    fprintf(fmeanisiid, "Mean: %f\n", mean);
    fprintf(fmeanisiid, "Standard Deviation: %f\n", stddev);
    fprintf(fmeanisiid, "\n");
    //fclose(degreefcid);

    fclose(fcdegreeid);
    fclose(firingcoherenceid);
    fclose(fisiid);
    fclose(fmeanisiid);

    // Notify the scheduler that the process has completed

    MPI_Send(&my_rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

  }

  ///////////////////////////////////////////////////////////////////////////////////////////////////////////

  // TO DO
  // Compute a stringbuilder for the file paths
  // Recieve all the variables and do the computations
  // Write the values to the files
  // Send the final computations to the coupling process
  if ((my_rank-1)%11 != 0 && my_rank!=0)
  {

    srand(time(NULL) + my_rank*world_size);

    // add safeguards from the recieved process

    // MPI_Send(&(FinalISI[0]), 100, MPI_DOUBLE, send_to_rank, 0, MPI_COMM_WORLD);
    // MPI_Send(&(no_spikes[0]), 100, MPI_INT, send_to_rank, 1, MPI_COMM_WORLD);
    // MPI_Send(&(connections[0]), 100, MPI_DOUBLE, send_to_rank, 2, MPI_COMM_WORLD);
    // MPI_Send(&tot_degree[0], 12, MPI_DOUBLE, send_to_rank, 3, MPI_COMM_WORLD);
    // MPI_Send(&count_degree[0], 12, MPI_INT, send_to_rank, 4, MPI_COMM_WORLD);
    // MPI_Send(&firing_coherence, 1, MPI_DOUBLE, send_to_rank, 5, MPI_COMM_WORLD);
    // MPI_Send(&my_rank, 1, MPI_INT, send_to_rank, 6, MPI_COMM_WORLD);
    // MPI_Send(&dvalue, 1, MPI_FLOAT, send_to_rank, 7, MPI_COMM_WORLD);
    // MPI_Send(&trial, 1, MPI_INT, send_to_rank, 8, MPI_COMM_WORLD);

    MPI_Recv(&(FinalISI[0]), 100, MPI_DOUBLE, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&(no_spikes[0]), 100, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&(connections[0]), 100, MPI_DOUBLE, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&tot_degree[0], 12, MPI_DOUBLE, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&count_degree[0], 12, MPI_INT, MPI_ANY_SOURCE, 4, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&firing_coherence, 1, MPI_DOUBLE, MPI_ANY_SOURCE, 5, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&rec_proc_rank, 1, MPI_INT, MPI_ANY_SOURCE, 6, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&dvalue, 1, MPI_FLOAT, MPI_ANY_SOURCE, 7, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&current_trial, 1, MPI_INT, MPI_ANY_SOURCE, 8, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    // printf("Rank is %d and trial number is %d and dvalue is %f\n", my_rank, current_trial + 1, dvalue);

    getcwd(file_path, 100);
    snprintf(coupling_path, 8, "%f", dvalue);
    strcat(file_path, "/");
    strcat(file_path,coupling_path);
    strcat(file_path, "/");
    sprintf(trial_path, "%d", current_trial);
    strcat(file_path, trial_path);
    strcat(file_path, "/");

    strcpy(base_path, file_path);

    strcat(file_path, "V_vs_T.txt");
    fV_vs_t_id = fopen(file_path, "w");

    strcpy(file_path, base_path);
    strcat(file_path, "Spiketime.txt");
    fspiketimeid = fopen(file_path, "w");

    strcpy(file_path, base_path);
    strcat(file_path, "Y.txt");
    yid = fopen(file_path, "w");

    strcpy(file_path, base_path);
    strcat(file_path, "VT.txt");
    fVTid = fopen(file_path, "w");

    strcpy(file_path, base_path);
    strcat(file_path, "RewiredMatrix.txt");
    fAdjacencymatrixId = fopen(file_path, "w");

    strcpy(file_path, base_path);
    strcat(file_path, "Randomnumbers.txt");
    frandomnumberId = fopen(file_path, "w");

    strcpy(file_path, base_path);
    strcat(file_path, "NoOfConnections.txt");
    fNoConnectionsId = fopen(file_path, "w");

    strcpy(file_path, base_path);
    strcat(file_path, "m_noise.txt");
    fm_noiseId = fopen(file_path, "w");

    strcpy(file_path, base_path);
    strcat(file_path, "mvalues.txt");
    fmvalueId = fopen(file_path, "w");

    strcpy(file_path, base_path);
    strcat(file_path, "hvalues.txt");
    fhvalueId = fopen(file_path, "w");

    strcpy(file_path, base_path);
    strcat(file_path, "nvalues.txt");
    fnvalueId = fopen(file_path, "w");

    strcpy(file_path, base_path);
    strcat(file_path, "output.txt");
    ftrial_output = fopen(file_path, "w");

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Second thread section, associate each file pointer with a thread for initialisation

    // These files are used for computing values of ISI, firing coeffiecients etc. for each trial
    // Files included are as follows -
    // fV_vs_t_id => V_vs_T.txt
    // fspiketimeid => Spiketime.txt
    // yid => Y.txt
    // fVTid => VT.txt
    // fAdjacencymatrixId => RewiredMatrix.txt
    // frandomnumberId => Randomnumbers.txt
    // fNoConnectionsId => NoOfConnections.txt
    // fm_noiseId => m_noise.txt
    // fmvalueId => mvalues.txt
    // fhvalueId => hvalues.txt
    // fnvalueId => nvalues.txt

    // Destroy threads and dynamic memory after allocation

    // thread = 0;
    // thread_count = 11;
    // pthread_t *thread_handles;
    // thread_handles = malloc(thread_count*sizeof(pthread_t));
    //
    // for (thread = 0; thread < thread_count; thread++)
    // {
    //   pthread_create(&thread_handles[thread], &attr, declare_trial_files, (void*) thread);
    // }

    // associate each block of initialisation with a seperate thread

    fprintf(ftrial_output, "p = %f\n", probability);

    int Done = 0;       //Flag

    //Initializing degree array
    for (i = 0; i < 20; i++)
    {
        degree[i] = 0;
    }

    //Initializing Edgematrix
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            edgematrix[i][j] = 0;
            edgematrix1[i][j] = 0;
        }
    }

    Nna = rhona * S;
    Nk = rhok * S;

    //Initializing V[j] Array

    //Constructing a ring lattice with k/2 connections on either side of each node

    for (i = 0; i < N; i++)
    {
        for (j = 1; j <= mean_degree/ 2; j++)
        {
            if ((i + j) >= N)
            {
                edgematrix[i][abs(N - (i + j))] = 1;
                edgematrix[i][abs(i - j)] = 1;
                edgematrix[abs(N - (i + j))][i] = 1;    //Since for an undirectional graph, edgematrix[i][j] = edgematrix[j][i]
                edgematrix[abs(i - j)][i] = 1;
            }
            else if ((i - j) < 0)
            {
                edgematrix[i][abs(N + (i - j))] = 1;
                edgematrix[i][abs(i + j)] = 1;
                edgematrix[abs(N + (i - j))][i] = 1;    //Since for an undirectional graph, edgematrix[i][j] = edgematrix[j][i]
                edgematrix[abs(i + j)][i] = 1;
            }
            else
            {
                edgematrix[i][abs(i + j)] = 1;
                edgematrix[i][abs(i - j)] = 1;
                edgematrix[abs(i + j)][i] = 1;          //Since for an undirectional graph, edgematrix[i][j] = edgematrix[j][i]
                edgematrix[abs(i - j)][i] = 1;
            }
        }
    }

    // for(thread = 0; thread < thread_count; thread++)
    // {
    //   pthread_join(thread_handles[thread], NULL);
    // }
    //
    // free(thread_handles);

    //End of second thread section

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////


    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            fprintf(fAdjacencymatrixId, "%f   ", edgematrix[i][j]);
            edgematrix1[i][j] = edgematrix[i][j];
        }
        fprintf(fAdjacencymatrixId, "\n");
    }

    fprintf(fAdjacencymatrixId, "\n\n\n\n");

    // Rewiring the system using the rewiring probability

    fprintf(frandomnumberId, "#N = %d, k = %d, p = %f \n", nodes, mean_degree, probability);
    fprintf(frandomnumberId, "S.No i    j    r    re   \n");

    for (i = 0; i < N - 1; i++)
    {
        for (j = i + 1; j < N; j++)
        {
            if (edgematrix[i][j] == 1)
            {
                randomnumber = randomnumbergenerator(max_randomnumber);         //Generate a random number and divide it by max_random number.
                randomnumber = (float)randomnumber / (float)max_randomnumber;   //If normalized value is less than p, remove connection
                if (randomnumber < probability)
                {
                    edgematrix[i][j] = 0;
                    edgematrix[j][i] = 0;
                    Done = 0;
                    while (Done != 1)
        //Done is a flag variable. rewiringnumber stores the index of a random neuron in the network to which neuron 'i' gets rewired to.
        //Self loops and redundant connections are avoided
        //Hence, once we establish a connection which is not a self loop or a redundant connection, Done becomes 1 and we get out of the loop
                    {
                        rewiringnumber = randomnumbergenerator(N - 1);
                        if (rewiringnumber != i && edgematrix[i][rewiringnumber] == 0)  //Eliminating self-loops & redundant connections
                        {
                            edgematrix[i][rewiringnumber] = 1;
                            edgematrix[rewiringnumber][i] = 1;  //Since for an undirectional graph, edgematrix[i][j] = edgematrix[j][i]
                            fprintf(frandomnumberId, "%d    %d    %d    %f    %d\n", 0, i, j, randomnumber, rewiringnumber);
                            Done = 1;
                        }
                        else
                        {
                            continue;
                        }
                    }
                }
            }
            else
                continue;
        }
        //printf("Node %d rewired. \n", i+1);
    }

    //Writing into files and printing
    //printf("Starting to write adjacency matrix into the file. \n\n");
    fprintf(fAdjacencymatrixId, "#N = %d, k = %d, p = %f \n", nodes, mean_degree, probability);
    fprintf(fNoConnectionsId, "#N = %d, k = %d, p = %f \n", nodes, mean_degree, probability);

    for (i = 0; i < N; i++)
    {
        no_of_connections = 0;
        for (j = 0; j < N; j++)
        {
            fprintf(fAdjacencymatrixId, "%f   ", edgematrix[i][j]); //Printing Adjacency Matrix of neuron
            if (edgematrix[i][j] == 1)
            {
                no_of_connections = no_of_connections + 1;  //Calculating number of connections of current neuron
            }
        }
        degree[no_of_connections]++;
        connections[i] = connections[i] + no_of_connections;
        fprintf(fAdjacencymatrixId, "\n");
        fprintf(fNoConnectionsId, "%d    %d\n", i + 1, no_of_connections);
    }
    //printf("Finished writing adjacency matrix into the file. \n\n");
    //printf("Finished printing number of connections per neuron. \n\n");

    for (i = 0; i < N; i++) // Iterating through all the nodes in the network
    {
        connections[i] = connections[i] / 2.0;
    }

    fprintf(fAdjacencymatrixId, "\n\n\n\n");
    int countno = 0;

    for (i = 0; i < N - 1; i++)
    {

        for (j = i + 1; j < N; j++)
        {
            if (edgematrix[i][j] == 0 && edgematrix1[i][j] == 1)
            {
                countno++;
            }
        }
    }

    fprintf(fAdjacencymatrixId, "%d", countno);

/*______________________________________________________________________________________________________________________________________*/
    fprintf(ftrial_output, "No of time steps: %d\n", n_steps);

    for (i = 0; i < N; i++)
    {
        V[i] = 0.0;
        k1[i] = 0.0;
        k2[i] = 0.0;
        k3[i] = 0.0;
        k4[i] = 0.0;
        m1[i] = 0.0;
        m2[i] = 0.0;
        m3[i] = 0.0;
        m4[i] = 0.0;
        n1[i] = 0.0;
        n2[i] = 0.0;
        n3[i] = 0.0;
        n4[i] = 0.0;
        h1[i] = 0.0;
        h2[i] = 0.0;
        h3[i] = 0.0;
        h4[i] = 0.0;
        coupling1[i] = 0.0;
        coupling2[i] = 0.0;
        coupling3[i] = 0.0;
        m_noise[i] = 0.0;
        n_noise[i] = 0.0;
        h_noise[i] = 0.0;

        alpha_m[i] = (0.1 * (25 - V[i])) / (exp((25 - V[i]) / 10) - 1);
        beta_m[i] = 4 * exp(-V[i] / 18);
        alpha_h[i] = 0.07 * exp(-V[i] / 20);
        beta_h[i] = 1 / (exp((30 - V[i]) / 10) + 1);
        alpha_n[i] = 0.01 * (10 - V[i]) / (exp((10 - V[i]) / 10) - 1);
        beta_n[i] = 0.125 * exp(-V[i] / 80);

        /*alpha_m[i] = (0.1*(V[i] + 40))/(-exp((-40 - V[i])/10) + 1);
         * beta_m[i] = 4*exp((-V[i] - 65)/18);
         * alpha_h[i] = 0.07*exp((-V[i]-65)/20);
         * beta_h[i] = 1/(exp((-35-V[i])/10) + 1);
         * alpha_n[i] = 0.01*(V[i] + 55)/(-exp((-55-V[i])/10) + 1);
         * beta_n[i] = 0.125*exp((-V[i]-65)/80);
         */

        m[i] = (alpha_m[i] / (alpha_m[i] + beta_m[i]));
        h[i] = (alpha_h[i] / (alpha_h[i] + beta_h[i]));
        n[i] = (alpha_n[i] / (alpha_n[i] + beta_n[i]));
    }

    for (i = 0; i < N; i++)
    {
        sum_coupling = 0.0;
        for (j = 0; j < N; j++)
        {
            sum_coupling = sum_coupling + (edgematrix[i][j] * (V[j] - V[i]));
        }
        coupling[i] = sum_coupling;
    }
    // printf("Done #1\n");
    //Starting calculation using RK4
    for (l = 0; l <= n_steps; l++)
    {
        //printf("%f\n",l*dt );
        fprintf(fV_vs_t_id, "%f\t", l * dt);
        fprintf(fmvalueId, "%f\n", l * dt);
        fprintf(fhvalueId, "%f\n", l * dt);
        fprintf(fnvalueId, "%f\n", l * dt);
        fprintf(fm_noiseId, "%f\t", l * dt);
        for (j = 0; j < N; j++)
        {
            alpha_m[j] = (0.1 * (25 - V[i])) / (exp((25 - V[i]) / 10) - 1);
            beta_m[j] = 4 * exp(-V[i] / 18);
            alpha_h[j] = 0.07 * exp(-V[i] / 20);
            beta_h[j] = 1 / (exp((30 - V[i]) / 10) + 1);
            alpha_n[j] = 0.01 * (10 - V[i]) / (exp((10 - V[i]) / 10) - 1);
            beta_n[j] = 0.125 * exp(-V[i] / 80);

            m_noise[j] = sqrt(((2 * alpha_m[j] * beta_m[j]) / (Nna * xna * (alpha_m[j] + beta_m[j])))) * gaussrandm();
            fprintf(fm_noiseId, "%f\t", m_noise[j]);
            n_noise[j] = sqrt(((2 * alpha_n[j] * beta_n[j]) / (Nk * xk * (alpha_n[j] + beta_n[j])))) * gaussrandn();
            h_noise[j] = sqrt(((2 * alpha_h[j] * beta_h[j]) / (Nna * xna * (alpha_h[j] + beta_h[j])))) * gaussrandh();
            k1[j] = dt * KV(coupling[j], V[j], m[j], h[j], n[j]);
            m1[j] = dt * (Km(V[j], m[j]) + m_noise[j]);
            h1[j] = dt * (Kh(V[j], h[j]) + h_noise[j]);
            n1[j] = dt * (Kn(V[j], n[j]) + n_noise[j]);
            coupling1[j] = dt * Kcoupling(V, edgematrix, nodes, k1, j);
        }
        fprintf(fm_noiseId, "\n");
        for (j = 0; j < N; j++)
        {
            k2[j] = dt * KV(coupling[j] + (0.5 * coupling1[j]), V[j] + (0.5 * k1[j]), m[j] + (0.5 * m1[j]), h[j] + (0.5 * h1[j]),n[j] + (0.5 * n1[j]));
            m2[j] = dt * (Km(V[j] + (0.5 * k1[j]), m[j] + (0.5 * m1[j])) + m_noise[j]);
            h2[j] = dt * (Kh(V[j] + (0.5 * k1[j]), h[j] + (0.5 * h1[j])) + h_noise[j]);
            n2[j] = dt * (Kn(V[j] + (0.5 * k1[j]), n[j] + (0.5 * n1[j])) + n_noise[j]);
            coupling2[j] = dt * Kcoupling(V, edgematrix, nodes, k2, j);
        }

        for (j = 0; j < N; j++)
        {

            k3[j] = dt * KV(coupling[j] + (0.5 * coupling2[j]), V[j] + (0.5 * k2[j]), m[j] + (0.5 * m2[j]), h[j] + (0.5 * h2[j]),n[j] + (0.5 * n2[j]));
            m3[j] = dt * (Km(V[j] + (0.5 * k2[j]), m[j] + (0.5 * m2[j])) + m_noise[j]);
            h3[j] = dt * (Kh(V[j] + (0.5 * k2[j]), h[j] + (0.5 * h2[j])) + h_noise[j]);
            n3[j] = dt * (Kn(V[j] + (0.5 * k2[j]), n[j] + (0.5 * n2[j])) + n_noise[j]);
            coupling3[j] = dt * Kcoupling1(V, edgematrix, nodes, k3, j);
        }

        for (j = 0; j < N; j++)
        {

            k4[j] = dt * KV(coupling[j] + coupling3[j], V[j] + k3[j], m[j] + m3[j], h[j] + h3[j], n[j] + n3[j]);
            m4[j] = dt * (Km(V[j] + k3[j], m[j] + m3[j]) + m_noise[j]);
            h4[j] = dt * (Kh(V[j] + k3[j], h[j] + h3[j]) + h_noise[j]);
            n4[j] = dt * (Kn(V[j] + k3[j], n[j] + n3[j]) + n_noise[j]);
        }

        for (j = 0; j < N; j++)
        {
            V[j] = V[j] + ((k1[j] + 2 * k2[j] + 2 * k3[j] + k4[j]) / 6);
            m[j] = m[j] + ((m1[j] + 2 * m2[j] + 2 * m3[j] + m4[j]) / 6);
            h[j] = h[j] + ((h1[j] + 2 * h2[j] + 2 * h3[j] + h4[j]) / 6);
            n[j] = n[j] + ((n1[j] + 2 * n2[j] + 2 * n3[j] + n4[j]) / 6);
            fprintf(fV_vs_t_id, "%f\t", V[j]);
            fprintf(fVTid, "%f\t", l * dt);
            fprintf(fVTid, "%f\n", V[j]);
            fprintf(fmvalueId, "%f\t", m[j]);
            fprintf(fhvalueId, "%f\t", h[j]);
            fprintf(fnvalueId, "%f\t", n[j]);
        }
        fprintf(fV_vs_t_id, "\n");
        fprintf(fmvalueId, "\n");
        fprintf(fhvalueId, "\n");
        fprintf(fnvalueId, "\n");
        for (i = 0; i < N; i++)
        {
            sum_coupling = 0.0;
            for (j = 0; j < N; j++)
            {
                sum_coupling = sum_coupling + (edgematrix[i][j] * (V[j] - V[i]));
            }
            coupling[i] = sum_coupling;
        }

    }

    fclose(fAdjacencymatrixId);
    fclose(fV_vs_t_id);
    fclose(fVTid);
    fclose(fNoConnectionsId);
    fclose(frandomnumberId);
    fclose(fmvalueId);
    fclose(fnvalueId);
    fclose(fhvalueId);
    //fclose(fMeanShortestPathId);
    //fclose(fClusteringCoeffId);
//________________________________________________________________________________________________________________________________________
    // printf("Done #2\n");
//Calculation of ISI, spike times, firing correlation, average firing coherence etc.
    double f_correlation[N][N];
    float tmp0 = 0.0;
    float tmp1 = 0.0;
    int s = 0;
    int a = 0;
    int b = 0;
    int c = 0;
    double z = 0.0;
    int smax = 0;
    char ignore[1024];
    double t1 = 0.0;
    double t2 = timewell;
    int no_cycles = (int)(time_max / timewell);
    double sum_total = 0.0;

    for (i = 0; i < N; i++)
    {
        //no_spikes[i] = 0;
        for (j = 0; j < N; j++)
        {
            f_correlation[i][j] = 0.0;
        }
    }

    strcpy(file_path, base_path);
    strcat(file_path, "VT.txt");
    fVTid = fopen(file_path, "r");

    float voltagematrix[n_steps];
    float timematrix[n_steps];

    for (i = 0; i < n_steps; i++)
    {
        voltagematrix[i] = 0.0;
        timematrix[i] = 0.0;
    }

    float tspike[N][n_steps / n_one_cycle];

    for (i = 0; i < N; i++)
        for (j = 0; j < (n_steps / n_one_cycle); j++)
            tspike[i][j] = 0;
    // printf("Done #3\n");
    for (i = 0; i < N; i++)
    {
        //countnotimes = 0;
        s = 0;
        tmp0 = 0.0;
        tmp1 = 0.0;
        // printf("loop0\n");
        for (j = 0; j < i; j++)
        {
            fgets(ignore, sizeof(ignore), fVTid);
            // printf("loop1\n");
        }
        for (l = 0; l < n_steps; l++)
        {
            fscanf(fVTid, "%f %f", &timematrix[l], &voltagematrix[l]);  // reading from file
            // printf("loop2\n");
            for (a = 0; a < N; a++)
            {
                fgets(ignore, sizeof(ignore), fVTid);
                // printf("loop3\n");
            }
        }
        //printf("%f\n",voltagematrix[1500]);
        for (b = 1; b <= n_steps - 1; b++)
        {
            //printf("Here\n");
            //countnotimes = countnotimes + 1;
            tmp0 = voltagematrix[b] - voltagematrix[b + 1];
            tmp1 = voltagematrix[b] - voltagematrix[b - 1];
            // printf("loop4\n");
            if (tmp0 > 0 && tmp1 > 0)
            {
                if (voltagematrix[b] > 70.0)
                {
                    tspike[i][s] = timematrix[b];
                    if (floor(tspike[i][s]) == floor(tspike[i][s - 1]) || floor(tspike[i][s]) == ceil(tspike[i][s - 1]))
                    {
                        tspike[i][s] = 0.0;
                        continue;
                    }
                    fprintf(fspiketimeid, "%f\t", tspike[i][s]);
                    // printf("loop5\n");
                    s = s + 1;
                    no_spikes[i] = s;
                    if (s > smax)
                    {
                        smax = s;
                    }
                }
            }
        }
        //printf("%d\n",countnotimes);
        fprintf(fspiketimeid, "\n");
        // printf("loop6\n");
        rewind(fVTid);
        // printf("loop7\n");
        //  printf("No of spikes of neuron %d: %d\n",i+1,no_spikes[i]);
    }
    // printf("Done #4\n");
    int flag = 0;

    for (i = 0; i < N; i++)
    {
        if (no_spikes[i] == 0)
            flag = 1;
    }

    // if (flag == 1)
    // {
    //     trial = trial - 1;
    //     printf("Re-simulating, no spikes detected\n");
    //     continue;
    // }

    double AvgISI[N];

    double ISI = 0.0;
    int count_isi = 0;

    for (i = 0; i < N; i++)
    {
        AvgISI[i] = 0.0;
    }

    for (i = 0; i < N; i++)
    {
        count_isi = 0;
        for (j = 1; j < smax; j++)
        {
            if (tspike[i][j] == 0.0)
            {
                continue;
            }
            ISI = tspike[i][j] - tspike[i][j - 1];
            AvgISI[i] = AvgISI[i] + ISI;
            count_isi = count_isi + 1;
        }
        AvgISI[i] = AvgISI[i] / count_isi;
    }

    for (i = 0; i < N; i++)
    {
        FinalISI[i] = FinalISI[i] + AvgISI[i];  // ISI averaged over all neurons
    }

    fclose(fspiketimeid);
    int Y[N][no_cycles];

    /*int **Y;
     * Y = (int **)malloc(N*sizeof(float));
     * for(c = 0; c < N; c++)
     * {
     * Y[c] = (int *)malloc(no_cycles * sizeof(int));
     * }
     */

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < no_cycles; j++)
        {
            Y[i][j] = 0;
        }
    }

    for (i = 0; i < N; i++)
    {
        for (a = 0; a < (n_steps / n_one_cycle); a++)   // n_steps/n_one_cycle is value of time bin
        {
            if (tspike[i][a] == 0)
                continue;
            int bin_number = (int)(floor(tspike[i][a]) / timewell); // timewell = n_steps/n_one_cycle
            Y[i][bin_number] = 1;
        }
    }

    for (i = 0; i < N; i++)
    {
        for (j = 0; j < no_cycles; j++)
        {
            fprintf(yid, "%d\t", Y[i][j]);
        }
        fprintf(yid, "\n");
    }

    int sum_spikes = 0;

    for (i = 0; i < N - 1; i++)
    {
        for (j = i + 1; j < N; j++)
        {
            for (l = 0; l < no_cycles; l++)
            {
                if (Y[i][l] == 1 && Y[j][l] == 1)
                {
                    sum_spikes = sum_spikes + 1;
                }
            }
            f_correlation[i][j] = (double)sum_spikes / sqrt(no_spikes[i] * no_spikes[j]);
            f_correlation[j][i] = (double)sum_spikes / sqrt(no_spikes[i] * no_spikes[j]);
            sum_spikes = 0;
        }
    }

    for (i = 0; i < N - 1; i++)
    {
        for (j = i + 1; j < N; j++)
        {
            sum_total = sum_total + f_correlation[i][j];
        }
    }

    sum_total = 2.0 * sum_total / (N * (N - 1));
    firing_coherence = firing_coherence + sum_total;    // population coherence
    fprintf(ftrial_output, "Firing Coherence: %f\n", sum_total);

    fclose(yid);

    int distance[N][N]; //Array containing shortest paths between all pairs of nodes // FLOYD WARSHALL Algorithm implementation
    //int i,j,l;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            if (edgematrix[i][j] == 0)
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

    int max_dist = 0;
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            //      printf("%d\t",distance[i][j]);
            if (distance[i][j] > max_dist)
                max_dist = distance[i][j];
        }
        //  printf("\n");
    }

    double fc_degree[max_dist]; // firing correlation by degree of separation.

    for (i = 0; i < max_dist; i++)
        fc_degree[i] = 0.0;

    double sum_fc_degree = 0.0;
    int count = 0;

    for (i = 1; i <= max_dist; i++)
    {
        sum_fc_degree = 0.0;
        count = 0;
        for (j = 0; j < N - 1; j++)
        {
            for (l = i + 1; l < N; l++)
            {
                if (distance[j][l] == i)
                {
                    sum_fc_degree = sum_fc_degree + f_correlation[j][l];
                    count = count + 1;
                }
            }
        }
        fc_degree[i - 1] = sum_fc_degree / count;
    }

    for (i = 0; i < max_dist; i++)
    {
        fprintf(ftrial_output, "%d:  %f\n", i + 1, fc_degree[i]);
        tot_degree[i] = tot_degree[i] + fc_degree[i];
        count_degree[i] = count_degree[i] + 1;
    }

    fclose(ftrial_output);
    MPI_Send(&(FinalISI[0]), 100, MPI_DOUBLE, rec_proc_rank, 0, MPI_COMM_WORLD);
    MPI_Send(&(no_spikes[0]), 100, MPI_INT, rec_proc_rank, 1, MPI_COMM_WORLD);
    MPI_Send(&(connections[0]), 100, MPI_DOUBLE, rec_proc_rank, 2, MPI_COMM_WORLD);
    MPI_Send(&tot_degree[0], 12, MPI_DOUBLE, rec_proc_rank, 3, MPI_COMM_WORLD);
    MPI_Send(&count_degree[0], 12, MPI_INT, rec_proc_rank, 4, MPI_COMM_WORLD);
    MPI_Send(&firing_coherence, 1, MPI_DOUBLE, rec_proc_rank, 5, MPI_COMM_WORLD);
    MPI_Send(&rec_proc_rank, 1, MPI_INT, rec_proc_rank, 6, MPI_COMM_WORLD);

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////

MPI_Finalize();
return 0;
}
//End of main function
