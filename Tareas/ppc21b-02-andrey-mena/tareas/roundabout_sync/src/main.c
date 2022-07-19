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

#include "array_int.h"
#include "simulation.h"

/**
 * @brief Main, se encarga de recibir los argumentos y leer los archivos,
 *        ademas crea el objeto simulation en el que envia los datos 
 *        necesarios para luego ser resuelto por sus subrutinas y 
 *        de esta manera completar la simulacion de una rotonda.
 * @param int argc
 * @param char* argv[]
 * @return 0
*/
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  int64_t min = 0;
  int64_t max = 0;
  int64_t verbose = 0;
  if (argc >= 2) {
    min = atoi(argv[1]);
  }
  if (argc >= 3) {
    max = atoi(argv[2]);
  }
  if (argc >= 4) {
    char str1[5];
    char str2[5];
    strcat(str1, argv[3]);  //NOLINT
    strcpy(str2, "-v");  //NOLINT
    if (strcmp(str1, str2)==0) { //NOLINT
      verbose = 1;
    }
  }
  FILE* input = stdin;
  int64_t segmentCapacity = -1;
  char string[10000][20];
  int index = 0;
  while (!feof(input)) {
    if (fscanf(input, "%"PRId64, &segmentCapacity) == 1 ||
      fscanf(input, "%s", string[index]) == 1) {
      if (strlen(string[index]) > (size_t)0) {
        index++;
      } else {
      }
    }
  }
  simulation_t* simulation = simulation_create();
  int64_t cars = index;
  shared_data_t* sharedData = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));
  if (simulation && sharedData) {
    simulation_run(string, cars, min, max, verbose,
      segmentCapacity, sharedData);
  } else {
    printf("Error, couldn't allocate simulation or sharedData");
  }
  simulation_destroy(simulation);
  //free(simulation);
  return error;
}
