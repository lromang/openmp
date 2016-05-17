#include <stdio.h>
#include <omp.h>

int threads = 4;

int main() {
  int i, id;  
  printf("Hello World\n");

  // Definir número de threads.
  omp_set_num_threads(threads);

  // Ejecutar for en parallelo.
  #pragma omp parallel for
  for(i=0; i<6; i++){
    //Obtener id del thread que ejecuta.
    id = omp_get_thread_num();
    printf("Iter:%d, Thread: %d \n", i, id);
  }
  /*
    Al especificar for en pragma,
    esta sección se debería ejecutar de
    manera secuencial
   */
  printf("GoodBye World\n");
}
