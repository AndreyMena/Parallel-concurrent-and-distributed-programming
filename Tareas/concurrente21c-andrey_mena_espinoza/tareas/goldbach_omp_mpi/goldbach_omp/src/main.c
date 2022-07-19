/** 
 * @copyright [2022] Andrey Esteban Mena Espinoza
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

#include "array_int.h"
#include "arrayThreeField_int.h"
#include "goldbachCalc.h"

/**
 * @brief Funcion que se encarga de leer los numeros o archivos por
 * la entrada estandar
 * @param shared_data
 * @return int
*/
int readStdin(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  int64_t number = 0;
  FILE* input = stdin;
  while (!feof(input)) {
    if (fscanf(input, "%"PRId64, &number) == 1) {
      if (!(number > INT64_MIN && number < INT64_MAX)) {
        break;
      }

      error = array_int_add(&shared_data->inputsArray, number);
      error = array_int_add(&shared_data->numOfSumsArray, 0);
    } else {
      break;
    }
  }
  return error;
}

/**
 * @brief Funcion que se encarga de imprimir los resultados de goldbach,
 * que se encuentran dentro de la estructura de datos.
 * @param shared_data
 * @return int
*/
int printResult(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  for (uint64_t indexInput = 0; indexInput < shared_data->inputsArray.size;
    indexInput++) {
    printf("%"PRId64": ", shared_data->inputsArray.elements[indexInput]);
    if (shared_data->numOfSumsArray.elements[indexInput] == 0) {
      printf("NA");
    } else {
      printf("%"PRId64" sums",
        shared_data->numOfSumsArray.elements[indexInput]);
      if (shared_data->inputsArray.elements[indexInput] < 0) {
        printf(": ");
        int contador = 0;
        int64_t indexArr = 0;
        while (contador !=
          shared_data->numOfSumsArray.elements[indexInput]) {
          if (shared_data->listArrayResult[indexInput].firstField[indexArr]
            != 0) {
            if (((shared_data->inputsArray.elements[indexInput] * -1) % 2)
              == 0) {
              printf("%"PRId64" + ",
                shared_data->listArrayResult[indexInput].firstField[
                indexArr]);
              printf("%"PRId64,
                shared_data->listArrayResult[indexInput].secondField[
                indexArr]);
              contador++;
            } else {
              printf("%"PRId64" + ",
                shared_data->listArrayResult[indexInput].firstField[
                  indexArr]);
              printf("%"PRId64" + ",
                shared_data->listArrayResult[indexInput].secondField[
                indexArr]);
              printf("%"PRId64,
                shared_data->listArrayResult[indexInput].thirdField[
                indexArr]);
              contador++;
            }
            if (contador != shared_data->numOfSumsArray.elements[indexInput]) {
              printf(", ");
            }
          }
          indexArr++;
        }
      }
    }
    printf("\n");
  }
  return error;
}

/**
 * @brief Main, redirige el programa
 * @param argc
 * @param argv[]
 * @return int
*/
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  shared_data_t* shared_data = shared_data_create();
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return EXIT_FAILURE;
    }
  }
  shared_data->thread_count = thread_count;

  error = readStdin(shared_data);
  goldbachCalc_t* goldbachCalc = goldbachCalc_create();


  // Start time.
  struct timespec start, finish;
  clock_gettime(CLOCK_MONOTONIC, &start);

  goldbachCalc_run(shared_data);  // To measure.
  printResult(shared_data);

  clock_gettime(CLOCK_MONOTONIC, &finish);
  double duration = finish.tv_sec - start.tv_sec +
    (finish.tv_nsec - start.tv_nsec) / 1e9;
  fprintf(stderr, "Execution time: %.9lf\n", duration);
  // Finish time.


  goldbachCalc_destroy(goldbachCalc);
  shared_data_destroy(shared_data);
  return error;
}
