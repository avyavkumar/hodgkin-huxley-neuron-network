#include "variables.h"

const int    mean_degree = 8;   //Define the mean degree of the network
const double timewell = 1.0;    //Timewell is a bin. You divide your timescale into 1ms bins and check if there are cases where two neurons fired together in the same bin.
const double time_max = 300.0;  //Total time for running the simulation
const double dt = 0.01;         //ms,  Time step size
const double Dmin = 0.01;       //Initial value of coupling strength
const double Dmax = 0.1;        //Final value of coupling strength
const double D_step = 0.01;     //Step size of coupling strength
const int    no_of_trials = 10; //Number of trials done

#define PI 3.1415926536
#define N 100                   //Define the number of nodes in the network

double xna = 1.0;
double xk = 0.7;
double rhona = 60.0;            //um-2
double rhok = 18.0;             //um-2
double S = 1.0;                 //Membrane patch size
double Nna;
double Nk;
double GNa = 120.0;
double GK = 36.0;
double GL = 0.3;
double VNa = 115.0;             //double VNa = 50.0 mV (This is according to the paper)
double VK = -12.0;              //double VK = -77.0 mV (This is according to the paper)
double VL = 10.5995;            //double VL = -54.4 mV (This is according to the paper)
double c = 1.0;
int n_one_cycle = 1000;         //No of time steps consumed in a spike. (approximation)
double D = 0.001;
double firing_coherence = 0.0;  //float *voltagematrix, **tspike, *timematrix,*vjunk,*tjunk;
int nodes = N;
double probablity = 0.05;       //Rewiring probablity
float randomnumber;             //Float because we need to calculate probability
int rewiringnumber;             //rewiringnumber is the node to which a broken connection is rewired.
int no_of_connections;          //no_of_connections is the variable used to count the number of connections on each node */
int max_randomnumber = 100000;  //Maximum Random Number
int degree[20];                 //Array containing number of neurons with n connections. For example, degree[2] will give number of neurons with 2 connections.
int no_spikes[N];
double edgematrix[N][N];        // This is the adjacency matrix used to store the topology (both initial and rewired)
double edgematrix1[N][N];       // In edgematrix1, I store the initial ring topology of the network. Used for comparision with the rewired matrix

//Defining parameters related to V[j] and gating variables

double sum_coupling = 0.0;
double V[N];
double m[N];
double n[N];
double h[N];
double coupling[N];
double alpha_m[N];
double beta_m[N];
double alpha_h[N];
double beta_h[N];
double alpha_n[N];
double beta_n[N];
double k1[N], m1[N], n1[N], h1[N], coupling1[N];
double k2[N], m2[N], n2[N], h2[N], coupling2[N];
double k3[N], m3[N], n3[N], h3[N], coupling3[N];
double k4[N], m4[N], n4[N], h4[N];

//double time_max = 100.0; //ms
#define n_steps = (int)time_max / dt;
double m_noise[N], n_noise[N], h_noise[N];
