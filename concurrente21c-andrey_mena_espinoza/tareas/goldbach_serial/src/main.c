/** 
 * @copyright [2022] Andrey Esteban Mena Espinoza
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>

#include "array_int.h"
#include "arrayThreeField_int.h"
#include "goldbachCalc.h"

/**
 * @brief Funcion que se encarga de leer los numeros o archivos por
 * la entrada estandar
 * @param data_structure
 * @return int
*/
int readStdin(data_structure_t* data_structure) {
  int error = EXIT_SUCCESS;
  int64_t number = 0;
  FILE* input = stdin;
  while (!feof(input)) {
    if (fscanf(input, "%"PRId64, &number) == 1) {
      if (!(number > -9223372036854775807 && number < 9223372036854775807)) {
        break;
      }

      error = array_int_add(&data_structure->inputsArray, number);

    } else {
      break;
    }
  }
  return error;
}

/**
 * @brief Funcion que se encarga de imprimir los resultados de goldbach,
 * que se encuentran dentro de la estructura de datos.
 * @param data_structure
 * @return int
*/
int printResult(data_structure_t* data_structure) {
  int error = EXIT_SUCCESS;
  for (uint64_t indexInput = 0; indexInput < data_structure->inputsArray.size;
    indexInput++) {
    printf("%"PRId64": ", data_structure->inputsArray.elements[indexInput]);
    if (data_structure->numOfSumsArray.elements[indexInput] == 0) {
      printf("NA");
    } else {
      printf("%"PRId64" sums",
        data_structure->numOfSumsArray.elements[indexInput]);
      if (data_structure->inputsArray.elements[indexInput] < 0) {
        printf(": ");
        for (uint64_t indexArr = 0; indexArr <
          data_structure->listArrayResult[indexInput].size; indexArr++) {
          if (((data_structure->inputsArray.elements[indexInput] * -1) % 2)
            == 0) {
            printf("%"PRId64" + ",
              data_structure->listArrayResult[indexInput].firstField[
              indexArr]);
            printf("%"PRId64,
              data_structure->listArrayResult[indexInput].secondField[
              indexArr]);
          } else {
            printf("%"PRId64" + ",
              data_structure->listArrayResult[indexInput].firstField[
                indexArr]);
            printf("%"PRId64" + ",
              data_structure->listArrayResult[indexInput].secondField[
              indexArr]);
            printf("%"PRId64,
              data_structure->listArrayResult[indexInput].thirdField[
              indexArr]);
          }
          if (indexArr != data_structure->listArrayResult[indexInput].size
            -1) {
            printf(", ");
          }
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
  argc = argc+0;  // Para evitar warning Wunused
  argv[0][0] = argv[0][0];  // Para evitar warning Wunused
  int error = EXIT_SUCCESS;
  data_structure_t* data_structure = data_structure_create();

  error = readStdin(data_structure);
  goldbachCalc_t* goldbachCalc = goldbachCalc_create();


  // Start time.
  struct timespec start, finish;
  clock_gettime(CLOCK_MONOTONIC, &start);

  goldbachCalc_run(data_structure);  // To measure.

  clock_gettime(CLOCK_MONOTONIC, &finish);
  double duration = finish.tv_sec - start.tv_sec +
    (finish.tv_nsec - start.tv_nsec) / 1e9;
  fprintf(stderr, "Execution time: %.9lf\n", duration);
  // Finish time.


  printResult(data_structure);

  goldbachCalc_destroy(goldbachCalc);
  data_structure_destroy(data_structure);
  return error;
}
