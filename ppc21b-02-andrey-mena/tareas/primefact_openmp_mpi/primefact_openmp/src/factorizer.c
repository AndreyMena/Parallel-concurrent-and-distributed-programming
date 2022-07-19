/** 
 * @copyright [2021] <Andrey Esteban Mena Espinoza>
*/
#include <pthread.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include "array_int.h"
#include "factorizer.h"
#include "math.h"

int factorizer_run(shared_data_t* sharedData) {
  int error = EXIT_SUCCESS;
  direct_threads(sharedData);
  factorizer_toString(sharedData);
  return error;
}

void direct_threads(shared_data_t* sharedData) {
  #pragma omp parallel for num_threads(sharedData->threadCount) \
    default(none) shared(sharedData)
  for (int64_t iterationCount = 0; iterationCount <
    (int)sharedData->numbers.size; iterationCount++) {
    int64_t integer = sharedData->numbers.elements[iterationCount];
    factorizeNumber(iterationCount, sharedData, integer);
  }
}

factorizer_t* factorizer_create(void) {
  factorizer_t* factorizer = (factorizer_t*)
    calloc(1, sizeof(factorizer_t));
  return factorizer;
}

int couldFactorize(int64_t position, shared_data_t* sharedData,
  int64_t integer) {
  if (sharedData->flags.elements[position] == 1) {
    char invalidNumber[20] = "invalid number";
    strcat(sharedData->salida[position], invalidNumber); //NOLINT
    return 0;
  } else {
    if (integer == 0 &&
      sharedData->flags.elements[position] == 0) {
      char NA[20] = "0: NA";
      strcat(sharedData->salida[position], NA); //NOLINT
      return 0;
    }
    if (integer == 1) {
      char one[20] = "1: NA";
      strcat(sharedData->salida[position], one); //NOLINT
      return 0;
    }
    if (integer < 0) {
      char number[500];
      sprintf(number, "%"PRId64": invalid number", integer); //NOLINT
      strcat(sharedData->salida[position], number); //NOLINT
      return 0;
    }
  }
  return 1;
}

int factorizeNumber(int64_t position, shared_data_t* sharedData,
  int64_t integer) {
  int64_t divider = 2;
  if (couldFactorize(position, sharedData, integer) != 1) {
    return 0;  // No puede factorizar
  }
  char number[200] = "";
  sprintf(number, "%"PRId64": ", integer); //NOLINT
  strcat(sharedData->salida[position], number); //NOLINT
  bool firstInteger = true;
  // int divider = 2;
  int64_t counter = 1;
  int64_t lastdivider = 0;
  do {
    if ( integer % divider != 0 ) {
      if (divider == 2) {
        divider = divider+1;
      } else {
        divider = divider+2;
      }
    } else {
      integer = integer/divider;
      if (divider != lastdivider) {
        if (firstInteger == false && counter > 1) {
          // char pow[100] = "^";
          strcat(sharedData->salida[position], /*pow*/"^"); //NOLINT
          char counterNum[200] = "";
          sprintf(counterNum,"%"PRId64"",counter); //NOLINT
          strcat(sharedData->salida[position], counterNum);  //NOLINT
          strcat(sharedData->salida[position], " "); //NOLINT
          char dividerNum[200] = "";
          sprintf(dividerNum,"%"PRId64"",divider); //NOLINT
          strcat(sharedData->salida[position], dividerNum); //NOLINT
          counter = 1;
        } else {
          if (firstInteger) {
            char dividerNum[200] = "";
            sprintf(dividerNum,"%"PRId64"",divider); //NOLINT
            strcat(sharedData->salida[position], dividerNum); //NOLINT
            firstInteger = false;
            counter = 1;
          } else {
            strcat(sharedData->salida[position], " "); //NOLINT
            char dividerNum[200] = "";
            sprintf(dividerNum,"%"PRId64"",divider); //NOLINT
            strcat(sharedData->salida[position], dividerNum); //NOLINT
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
    strcat(sharedData->salida[position], /*pow*/"^"); //NOLINT
    char counterNum[200] = "";
    sprintf(counterNum,"%"PRId64"",divider); //NOLINT
    strcat(sharedData->salida[position], counterNum); //NOLINT
  }
  return 1;
}

void factorizer_toString(shared_data_t* sharedData) {
  for (uint64_t index = 0; index < sharedData->numbers.size; index++) {
    printf("%s\n", sharedData->salida[index]);
  }
}

void factorizer_destroy(factorizer_t* factorizer) {
  assert(factorizer);
  free(factorizer);
}
