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
#include <time.h>

#include "factorizer.h"
#include "array_int.h"

/**
 * @brief startFactorizer, dirige la factorizacion
 * @param int64_t threadCount, Cantidad de hilos 
 * @param int64_t integer, numero a factorizar
 * @return 0
*/
void startFactorizer(int64_t threadCount, int64_t integer,
  factorizer_t* factorizer) {
  if (factorizer) {
    shared_data_t* sharedData = (shared_data_t*)
      calloc(1, sizeof(shared_data_t));
    if (sharedData) {
      sharedData->threadCount = threadCount;
      sharedData->integer = integer;
      array_int_t array;
      array_int_init(&array);
      sharedData->factors = array;
      int error = pthread_mutex_init(&sharedData->can_acces_array
        , NULL);
      if (error != EXIT_SUCCESS) {
        fprintf(stderr,
          "Error: Couldn't allocate factorizer object\n");
      }

      error = factorizer_run(sharedData);

      pthread_mutex_destroy(&sharedData->can_acces_array);
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
      fprintf(stderr, "Erro: invalid thread count\n");
      return EXIT_FAILURE;
    }
  }
  char string[100000] = "";

  struct timespec start, finish;
  clock_gettime(CLOCK_MONOTONIC, &start);
  while (!feof(input)) {
    factorizer_t* factorizer = factorizer_create();
    if (fscanf(input, "%"PRId64, &integer) == 1 ||
      fscanf(input, "%s", string) == 1) {
      if (strlen(string) > 0) {
        printf("invalid number\n");
        memset(string, 0, 100000);
      } else {
        if (integer <= -9223372036854775807 || integer >= 9223372036854775807) {
          printf("invalid number\n");
        } else {
          if (integer > 1) {
            startFactorizer(threadCount, integer, factorizer);
          }
          if (integer == 1 || integer == 0) {
            printf("%"PRId64": NA\n", integer);
          }
          if (integer < 0) {
            printf("%"PRId64": invalid number\n", integer);
          }
        }
      }
    }
    factorizer_destroy(factorizer);
  }
  clock_gettime(CLOCK_MONOTONIC, &finish);
  double duration = finish.tv_sec - start.tv_sec +
    (finish.tv_nsec - start.tv_nsec) / 1e9;
  printf("Execution time: %.9lf\n", duration);
  return error;
}
