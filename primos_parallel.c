//////////////////// Constantes y variables globales //////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>


static       int gProgress    = 0,
                 gPrimesFound = 0;       // Acumulador de número de primos encontrados
long             globalPrimes[1000000];  // Para almacenar los primos encontrados
int              CLstart, CLend;



///////////////////////////////////////////////////////////////////////////////
//
//  Imprime portcentaje de avance
//
///////////////////////////////////////////////////////////////////////////////

void ShowProgress( int val, int range )
{
    int percentDone = 0;
    gProgress++;
    percentDone = (int)((float)gProgress/(float)range *200.0f + 0.5f);
        if( percentDone % 10 == 0 )
        printf("\b\b\b\b%3d%%", percentDone);
}

///////////////////////////////////////////////////////////////////////////////
//
//  Busca factorres.  Devuelve "0" si los encuentra
//
///////////////////////////////////////////////////////////////////////////////

int TestForPrime(int val)
{
    int limit, factor = 3;
    limit = (long)(sqrtf((float)val)+0.5f);
    #pragma omp parallel shared(factor)
    while((factor <= limit) && (val % factor))
      #pragma omp critical
        factor = factor + 1;
    return (factor > limit);
}

void FindPrimes(int start, int end)
{
    // start siempre es non
    int i, range = end - start + 1;
    #pragma omp parallel for private(i) shared(gPrimesFound)
    for( i = start; i <= end; i += 2 )
    {
        if( TestForPrime(i) )
    #pragma omp critical
            globalPrimes[gPrimesFound++] = i;
        ShowProgress(i, range);
    }
}


int main(int argc, char **argv)
{
    int     start, end,i;
    clock_t before, after;
    int     numThreads   = 4;

    // Set number of threads.
    omp_set_num_threads(numThreads);
    if( argc == 3 )
    {
        CLstart = atoi(argv[1]);
        CLend   = atoi(argv[2]);
    }
    else
    {
        printf("Uso:- %s <rango inicio> <rango fin>\n", argv[0]);
        exit(-1);
    }

    if (CLstart > CLend) {
        printf("Valor inicial debe ser menor o igual al valor final\n");
        exit(-1);
    }
    if (CLstart < 1 && CLend < 2) {
                printf("El rango de bÃºsqueda debe estar formado por enteros positivos\n");
                exit(-1);
    }
    start = CLstart;
    end = CLend;

    if (CLstart < 2)
        start = 2;
    if (start <= 2)
        globalPrimes[gPrimesFound++] = 2;

    if((start % 2) == 0 )
        start = start + 1;  // Asegurar que iniciamos con un nÃºmero impar


    before = clock();
    FindPrimes(start, end);
    after = clock();

    printf("\n\nSe encontraron %8d primos entre  %6d y %6d en %7.2f secs\n",
           gPrimesFound, CLstart, CLend, (float)(after - before)/ CLOCKS_PER_SEC);

}
