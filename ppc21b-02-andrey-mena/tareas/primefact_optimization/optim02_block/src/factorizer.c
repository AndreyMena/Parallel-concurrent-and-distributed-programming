/** 
 * @copyright [2021] <Andrey Esteban Mena Espinoza>
*/
#include <pthread.h>
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


// thread privateData
typedef struct privateData {
  uint64_t threadNumber;
  int64_t integer;
  int64_t positionInTheArray;
  shared_data_t* shared_data;
  int64_t min;
  int64_t max;
}privateData_t;

int factorizer_run(shared_data_t* sharedData) {
  int error = EXIT_SUCCESS;
  pthread_t* threads = (pthread_t*)
    malloc(sharedData->threadCount * sizeof(pthread_t));
  privateData_t* privateData = (privateData_t*)
    calloc(sharedData->threadCount, sizeof(privateData_t));
  if (threads && privateData) {
    direct_threads(threads, sharedData, privateData);
  } else {
    printf("Error, couldn't allocate memory");
  }
  factorizer_toString(sharedData);
  free(privateData);
  free(threads);
  return error;
}

void direct_threads(pthread_t* threads , shared_data_t* sharedData,
  privateData_t* privateData) {
  int error = EXIT_SUCCESS;
  int64_t min = 0;
  uint64_t jobDivision = (int)sharedData->numbers.size /
    sharedData->threadCount;
  int64_t max = jobDivision;
  for (uint64_t threadNumber = 0; threadNumber < sharedData->threadCount
    ; threadNumber++) {
    // Actualizacion de datos para siguiente hilo:
    if (threadNumber == sharedData->threadCount-1) {
      privateData[threadNumber].threadNumber = threadNumber;
      privateData[threadNumber].positionInTheArray = min;
      privateData[threadNumber].min = min;
      privateData[threadNumber].max = (int)sharedData->numbers.size;
      privateData[threadNumber].integer =
        sharedData->numbers.elements[min];
      privateData[threadNumber].shared_data = sharedData;
    } else {
      privateData[threadNumber].threadNumber = threadNumber;
      privateData[threadNumber].positionInTheArray = min;
      privateData[threadNumber].min = min;
      privateData[threadNumber].max = max;
      privateData[threadNumber].integer =
        sharedData->numbers.elements[min];
      privateData[threadNumber].shared_data = sharedData;
      min = max;
      max = max+jobDivision;
    }
  }
  for (uint64_t threadNumber = 0; threadNumber < sharedData->threadCount
    ; threadNumber++) {
    error = pthread_create(&threads[threadNumber], NULL
      , factorize_all_numbers, &privateData[threadNumber]);
    if (error != EXIT_SUCCESS) {
      fprintf(stderr, "Error, couldn't allocate thread");
    }
  }
  for (uint64_t threadNumber = 0
    ; threadNumber < sharedData->threadCount; threadNumber++) {
      pthread_join(threads[threadNumber], NULL);
  }
}

factorizer_t* factorizer_create(void) {
  factorizer_t* factorizer = (factorizer_t*)
    calloc(1, sizeof(factorizer_t));
  return factorizer;
}

void* factorize_all_numbers(void* data) {
  privateData_t* privateData = (privateData_t*) data;
  shared_data_t* sharedData = privateData->shared_data;
  while (privateData->min < privateData->max) {
    factorizeNumber(privateData, sharedData);
    privateData->min++;
    privateData->positionInTheArray++;
    privateData->integer =
      sharedData->numbers.elements[privateData->positionInTheArray];
  }
  return NULL;
}

int couldFactorize(privateData_t* privateData, shared_data_t* sharedData) {
  if (sharedData->flags.elements[privateData->positionInTheArray] == 1) {
    char invalidNumber[20] = "invalid number";
    strcat(sharedData->salida[privateData->positionInTheArray], invalidNumber); //NOLINT
    return 0;
  } else {
    if (privateData->integer == 0 &&
      sharedData->flags.elements[privateData->positionInTheArray] == 0) {
      char NA[20] = "0: NA";
      strcat(sharedData->salida[privateData->positionInTheArray], NA); //NOLINT
      return 0;
    }
    if (privateData->integer == 1) {
      char one[20] = "1: NA";
      strcat(sharedData->salida[privateData->positionInTheArray], one); //NOLINT
      return 0;
    }
    if (privateData->integer < 0) {
      char number[500];
      sprintf(number, "%"PRId64": invalid number", privateData->integer); //NOLINT
      strcat(sharedData->salida[privateData->positionInTheArray], number); //NOLINT
      return 0;
    }
  }
  return 1;
}

int factorizeNumber(privateData_t* privateData, shared_data_t* sharedData) {
  int64_t integer = privateData->integer;
  int64_t divider = 2;
  if (couldFactorize(privateData, sharedData) != 1) {
    return 0;  // No puede factorizar
  }
  char number[200] = "";
  sprintf(number, "%"PRId64": ", privateData->integer); //NOLINT
  strcat(sharedData->salida[privateData->positionInTheArray], number); //NOLINT
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
          strcat(sharedData->salida[privateData->positionInTheArray], /*pow*/"^"); //NOLINT
          char counterNum[200] = "";
          sprintf(counterNum,"%"PRId64"",counter); //NOLINT
          strcat(sharedData->salida[privateData->positionInTheArray], counterNum);  //NOLINT
          strcat(sharedData->salida[privateData->positionInTheArray], " "); //NOLINT
          char dividerNum[200] = "";
          sprintf(dividerNum,"%"PRId64"",divider); //NOLINT
          strcat(sharedData->salida[privateData->positionInTheArray], dividerNum); //NOLINT
          counter = 1;
        } else {
          if (firstInteger) {
            char dividerNum[200] = "";
            sprintf(dividerNum,"%"PRId64"",divider); //NOLINT
            strcat(sharedData->salida[privateData->positionInTheArray], dividerNum); //NOLINT
            firstInteger = false;
            counter = 1;
          } else {
            strcat(sharedData->salida[privateData->positionInTheArray], " "); //NOLINT
            char dividerNum[200] = "";
            sprintf(dividerNum,"%"PRId64"",divider); //NOLINT
            strcat(sharedData->salida[privateData->positionInTheArray], dividerNum); //NOLINT
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
    strcat(sharedData->salida[privateData->positionInTheArray], /*pow*/"^"); //NOLINT
    char counterNum[200] = "";
    sprintf(counterNum,"%"PRId64"",divider); //NOLINT
    strcat(sharedData->salida[privateData->positionInTheArray], counterNum); //NOLINT
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
