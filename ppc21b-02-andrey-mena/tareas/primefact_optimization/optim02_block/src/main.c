/** 
 * @copyright [2021] <Andrey Esteban Mena Espinoza>
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include "factorizer.h"
#include "array_int.h"

/**
 * @brief startFactorizer, dirige la factorizacion
 * @param int64_t threadCount, Cantidad de hilos 
 * @param int64_t integer, numero a factorizar
 * @return 0
*/
void initilizeFactorizations(int64_t threadCount,
  factorizer_t* factorizer, array_int_t entradas, array_int_t flags) {
  if (factorizer) {
    shared_data_t* sharedData = (shared_data_t*)
      calloc(1, sizeof(shared_data_t));
    if (sharedData) {
      sharedData->threadCount = threadCount;
      sharedData->numbers = entradas;
      sharedData->flags = flags;
      if ((size_t)threadCount > sharedData->numbers.size) {
        threadCount = (uint64_t)sharedData->numbers.size;
        sharedData->threadCount = threadCount;
      }
      int error = pthread_mutex_init(&sharedData->can_acces_array
        , NULL);
      if (error != EXIT_SUCCESS) {
        fprintf(stderr,
          "Error: Couldn't allocate factorizer object\n");
      }

      struct timespec start, finish;
      clock_gettime(CLOCK_MONOTONIC, &start);

      error = factorizer_run(sharedData);   // Instruccion a medir

      clock_gettime(CLOCK_MONOTONIC, &finish);
      double duration = finish.tv_sec - start.tv_sec +
        (finish.tv_nsec - start.tv_nsec) / 1e9;
      fprintf(stderr, "Execution time: %.9lf\n", duration);

      pthread_mutex_destroy(&sharedData->can_acces_array);
      array_int_destroy(&sharedData->flags);
      array_int_destroy(&sharedData->numbers);
      // free(sharedData->salida);  // No se libera ya que no es un malloc
      free(sharedData);
    } else {
      fprintf(stderr, "Error: Couldn't allocate shared data\n");
    }
  } else {
    fprintf(stderr, "Error: Couldn't allocate factorizer object\n");
  }
}

/**
 * @brief Main, se encarga de controlar las factorizacions
 * @details 
 *        Pregunta por cantidad de hilos a usar, recibe los enteros o 
 *        los lee en caso de ser introducidos por medio de 
 *        archivos de prueba y se encarga de redirigir y factorizar los casos en
 *        que puede hacerlo, para que de esta manera nuestro factorizer
 *        reciba unicamente los numero que puede factorizar, 
 * @param void
 * @return 0
*/
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  FILE* input = stdin;
  int64_t integer = -1;
  uint64_t threadCount = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc >= 2) {
    if (sscanf(argv[1], "%" SCNu64, &threadCount) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return EXIT_FAILURE;
    }
  }
  array_int_t entradas;
  array_int_t flags;
  array_int_init(&entradas);
  array_int_init(&flags);
  char string[100000] = "";
  factorizer_t* factorizer = factorizer_create();
  while (!feof(input)) {
    if (fscanf(input, "%"PRId64, &integer) == 1 ||
      fscanf(input, "%s", string) == 1) {
      if (strlen(string) > 0) {
        array_int_add(&flags, 1/*Bandera para "invalid number" SIN NUMERO*/);
        array_int_add(&entradas, 0);
        memset(string, 0, 100000);
      } else {
        if (integer <= -9223372036854775807 || integer >= 9223372036854775807) {
          array_int_add(&flags, 1/*Bandera "invalid number" por overflow*/);
          array_int_add(&entradas, 0);
        } else {
          array_int_add(&entradas, integer);
          array_int_add(&flags, 0/*Bandera para impresion con numero*/);
        }
      }
    }
  }
  initilizeFactorizations(threadCount, factorizer, entradas, flags);
  factorizer_destroy(factorizer);
  return error;
}
