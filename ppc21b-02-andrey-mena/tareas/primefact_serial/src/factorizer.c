#include "factorizer.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
/** 
 * @copyright [2021] <Andrey Esteban Mena Espinoza>
*/
typedef struct dataBase {
  char outputs[1000][1000];
  int64_t posicionDataBase;
}factorizer_t;

factorizer_t* factorizer_create(void) {
  factorizer_t* factorizer = (factorizer_t*)
    calloc(1, sizeof(factorizer_t));
  return factorizer;
}

void factorizer_factorize(int64_t integer) {
  printf("%"PRId64": ", integer);
  bool firstInteger = true;
  int64_t divider = 2;
  int64_t counter = 1;
  int64_t lastdivider = 0;
  do {
    if ( integer % divider != 0 ) {
      divider = divider+1;
    } else {
      integer = integer/divider;
      if (divider != lastdivider) {
        if (firstInteger == false && counter > 1) {
          printf("^%"PRId64, counter);
          printf(" %"PRId64, divider);
          counter = 1;
        } else {
          if (firstInteger) {
            printf("%"PRId64, divider);
            firstInteger = false;
            counter = 1;
          } else {
            printf(" %"PRId64, divider);
          }
        }
      } else {
        counter++;
      }
      lastdivider = divider;
      divider = 2;
    }
  } while (integer != 1);
  if (counter > 1) {
    printf("^%"PRId64, counter);
  }
  printf("\n");
}

void factorizer_destroy(factorizer_t* factorizer) {
  assert(factorizer);
  free(factorizer);
}
