#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "factorizer.h"
/** 
 * @copyright [2021] <Andrey Esteban Mena Espinoza>
*/

/**
 * @brief Main, se encarga de controlar las factorizacions
 * @details 
 *        Recibe los enteros o los lee en caso de ser introducidos por medio de 
 *        archivos de prueba y se encarga de redirigir y factorizar los casos en
 *        que puede hacerlo, para que de esta nuestro factorizer reciba unicamente
 *        los numero que puede factorizar 
 * @param void
 * @return 0
*/
int main(void) {
  FILE* input = stdin;
  int64_t integer = -1;
  factorizer_t* factorizer = factorizer_create();

  char string[100000];
  struct timespec start, finish;
  clock_gettime(CLOCK_MONOTONIC, &start);
  while (!feof(input)) {
    if (fscanf(input, "%"PRId64, &integer) == 1 ||
      fscanf(input, "%s", string) == 1) {
      if (strlen(string) > 0) {
        printf("invalid number\n");
        memset(string, 0, 100000);
      } else {
        if (integer >= 9223372036854775807 ||integer <= -9223372036854775807) {
          printf("invalid number\n");
        } else {
          if (integer > 1) {
            if (factorizer) {
              factorizer_factorize(integer);
            } else {
              fprintf(stderr, "Error: Couldn't allocate factorizer object\n");
            }
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
  }
  clock_gettime(CLOCK_MONOTONIC, &finish);
  double duration = finish.tv_sec - start.tv_sec + 
    (finish.tv_nsec - start.tv_nsec) / 1e9;
  printf("Execution time: %.9lf\n",duration);
  factorizer_destroy(factorizer);
  return 0;
}
