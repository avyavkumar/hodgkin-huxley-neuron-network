#ifndef FUNCTION_H
#define FUNCTION_H

#define N 100

double KV(double C, double V, double m, double h, double n);
double Km(double V, double m);
double Kh(double V, double h);
double Kn(double V, double n);
double Kcoupling(double a[N], double b[][N], int m, double k[N], int index);
double Kcoupling1(double a[N], double b[][N], int m, double k[N], int index);
int randomnumbergenerator(int max_num);
double gaussrandm();
double gaussrandh();
double gaussrandn();
double clusteringcoefficient(int a[][N], int m);
double meanshortest(int a[][N], int m);

#endif
