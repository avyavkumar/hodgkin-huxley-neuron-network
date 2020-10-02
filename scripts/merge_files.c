#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv)
{
  if (argc != 4)
  {
    fprintf(stderr, "argument mismatch\n");
    return 1;
  }
  else
  {
    float dmin = strtof(argv[1], NULL);
    float dmax = strtof(argv[2], NULL);
    float dstep = strtof(argv[3], NULL);
    float i;
    float dfrac = 0.00001;
    char *input_path = (char *)malloc(100*sizeof(char));
    char *output_path = (char *)malloc(100*sizeof(char));
    char word;
    char *base_path = (char *)malloc(100*sizeof(char));
    char *args = (char*)malloc(8*sizeof(char));
    getcwd(base_path, 100);
    strcat(base_path, "/");
    strcpy(output_path, base_path);
    strcpy(input_path, base_path);
    FILE *foutput;
    FILE *finput;

    for (i = dmin; i <= dmax + dfrac; i+= dstep)
    {
      snprintf(args, 8, "%f", i);
      strcat(input_path, args);
      strcat(input_path, "/");
      strcat(input_path, "FiringCoherenceVaryingDs(100,8)(0.9;0.6).txt");
      strcat(output_path, "FiringCoherenceVaryingDs(100,8)(0.9;0.6).txt");
      if (i == dmin)
        foutput = fopen(output_path, "w");
      else
        foutput = fopen(output_path, "a");
      finput = fopen(input_path, "r");
      while(1)
      {
        word = fgetc(finput);
        if (word == EOF)
          break;
        fputc(word, foutput);
      }
      fclose(foutput);
      fclose(finput);
      strcpy(input_path, base_path);
      strcpy(output_path, base_path);
    }

    for (i = dmin; i <= dmax + dfrac; i+= dstep)
    {
      snprintf(args, 8, "%f", i);
      strcat(input_path, args);
      strcat(input_path, "/");
      strcat(input_path, "FCDegreeVaryingD(100,8)(0.9;0.6).txt");
      strcat(output_path, "FCDegreeVaryingD(100,8)(0.9;0.6).txt");
      if (i == dmin)
        foutput = fopen(output_path, "w");
      else
        foutput = fopen(output_path, "a");
      finput = fopen(input_path, "r");
      while(1)
      {
        word = fgetc(finput);
        if (word == EOF)
          break;
        fputc(word, foutput);
      }
      fclose(foutput);
      fclose(finput);
      strcpy(input_path, base_path);
      strcpy(output_path, base_path);
    }

    for (i = dmin; i <= dmax + dfrac; i+= dstep)
    {
      snprintf(args, 8, "%f", i);
      strcat(input_path, args);
      strcat(input_path, "/");
      strcat(input_path, "ISI_no_connections(100,8)(0.9;0.6).txt");
      strcat(output_path, "ISI_no_connections(100,8)(0.9;0.6).txt");
      if (i == dmin)
        foutput = fopen(output_path, "w");
      else
        foutput = fopen(output_path, "a");
      finput = fopen(input_path, "r");
      while(1)
      {
        word = fgetc(finput);
        if (word == EOF)
          break;
        fputc(word, foutput);
      }
      fclose(foutput);
      fclose(finput);
      strcpy(input_path, base_path);
      strcpy(output_path, base_path);
    }

    for (i = dmin; i <= dmax + dfrac; i+= dstep)
    {
      snprintf(args, 8, "%f", i);
      strcat(input_path, args);
      strcat(input_path, "/");
      strcat(input_path, "MeanISI(100,8)(0.9;0.6).txt");
      strcat(output_path, "MeanISI(100,8)(0.9;0.6).txt");
      if (i == dmin)
        foutput = fopen(output_path, "w");
      else
        foutput = fopen(output_path, "a");
      finput = fopen(input_path, "r");
      while(1)
      {
        word = fgetc(finput);
        if (word == EOF)
          break;
        fputc(word, foutput);
      }
      fclose(foutput);
      fclose(finput);
      strcpy(input_path, base_path);
      strcpy(output_path, base_path);
    }

    return 0;
  }
}
