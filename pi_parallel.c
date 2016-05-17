#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

long long num_steps = 2000000000;
double step;

int main(int argc, char* argv[])
{
  clock_t start, stop;
  double pi, sum, x;
  int i, id;
  sum  = 0.0;
  step = 1./(double)num_steps;
  
  // Número de threads
  omp_set_num_threads(4);
  start = clock();  

  /*#pragma omp parallel for private(i) shared(sum, step)
  for (i=0; i<num_steps; i++)
    {
      #pragma omp critical
      sum = sum + 4.0/(1. + (((i + .5)*step)*((i + .5)*step)));
    }
  */
  #pragma omp parallel for reduce(:= sum)
  for(i = 0; i < num_steps; i ++){
      sum = sum + 4.0/(1. + (((i + .5)*step)*((i + .5)*step)));
  }
  pi   = sum*step;
  stop = clock();

  printf("El valor calculado de Pi es: %15.12f\n",pi);
  printf("El tiempo requerido para calcular Pi fue %f milisegundos\n",
         ((double)(stop - start)/1000.0));
  return 0;
}
