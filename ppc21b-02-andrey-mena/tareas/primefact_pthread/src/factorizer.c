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

// thread privateData
typedef struct privateData {
  uint64_t threadNumber;
  int64_t minLim;
  int64_t maxLim;
  int64_t integer;
  shared_data_t* shared_data;
}privateData_t;

int factorizer_run(shared_data_t* sharedData) {
  int error = EXIT_SUCCESS;
  pthread_t* threads = (pthread_t*)
    malloc(sharedData->threadCount * sizeof(pthread_t));
  privateData_t* privateData = (privateData_t*)
    calloc(sharedData->threadCount, sizeof(privateData_t));

  if (threads && privateData) {
    int64_t minLimit = 0;
    int64_t maxLimit = sharedData->integer;
    const int64_t int64 = sharedData->integer;
    int64_t jobDivision = int64/sharedData->threadCount;
    int64_t firstJobDivision = jobDivision;

    directJobs(threads, sharedData, privateData, maxLimit, jobDivision,
      minLimit, firstJobDivision);
  } else {
    printf("Error, couldn't allocate memory");
  }
  factorizer_toString(sharedData);
  array_int_destroy(&sharedData->factors);
  return error;
}

void directJobs(pthread_t* threads , shared_data_t* sharedData,
  privateData_t* privateData, int64_t maxLimit,
    int64_t jobDivision, int64_t minLimit, int64_t firstJobDivision) {
  int error = EXIT_SUCCESS;
  for (uint64_t threadNumber = 0; threadNumber < sharedData->threadCount
    ; ++threadNumber) {
    privateData[threadNumber].threadNumber = threadNumber;
    privateData[threadNumber].integer = sharedData->integer;

    // Logica para dividir el trabajo de hilos,
    maxLimit = jobDivision;
    if (threadNumber+1 == sharedData->threadCount) {
      privateData[threadNumber].maxLim = sharedData->integer;
    } else {
      privateData[threadNumber].maxLim = maxLimit;
    }
    privateData[threadNumber].minLim = minLimit;
    minLimit = jobDivision;
    jobDivision = jobDivision+firstJobDivision;

    privateData[threadNumber].shared_data = sharedData;
    // Fin de division del trabajo

    error = pthread_create(&threads[threadNumber], NULL
      , factorizer_findFactors, &privateData[threadNumber]);
    if (error != EXIT_SUCCESS) {
      fprintf(stderr, "Error, couldn't allocate thread");
    }
  }
  for (uint64_t threadNumber = 0
    ; threadNumber < sharedData->threadCount; ++threadNumber) {
    pthread_join(threads[threadNumber], NULL);
  }
  free(privateData);
  free(threads);
}

factorizer_t* factorizer_create(void) {
  factorizer_t* factorizer = (factorizer_t*)
    calloc(1, sizeof(factorizer_t));
  return factorizer;
}

void* factorizer_findFactors(void* data) {
  privateData_t* privateData = (privateData_t*) data;
  shared_data_t* sharedData = privateData->shared_data;
  int64_t divider = privateData->minLim;

  if (privateData->minLim == 0) {
    divider = 1;
  }
  while (divider <= privateData->maxLim) {
    divider++;
    if (privateData->integer % divider == 0) {
      if (factorizer_isPrime(divider) == 1) {
        // Region critica, pueden acceder al arreglo varios
        // threads al mismo tiempo. Usamos mutex para serializar.
        pthread_mutex_lock(&sharedData->can_acces_array);
        int canAdd = verifyRepetition(sharedData, divider);
        if (canAdd == 1) {
          array_int_add(&sharedData->factors, divider);
        }
        pthread_mutex_unlock(&sharedData->can_acces_array);
      }
    }
  }
  return NULL;
}

int verifyRepetition(shared_data_t* sharedData, int64_t divider) {
  for (size_t index = 0; index < sharedData->factors.size; index++) {
    if (sharedData->factors.elements[index] == divider) {
      return 0;
    }
  }
  return 1;
}

int factorizer_isPrime(int64_t divider) {
  int64_t counter = 0;
  for (int64_t i = 2; i <= divider/2; i++) {
    if (divider%i == 0) {
      counter++;
      return 0;
    }
  }
  if (counter == 0 && divider != 1) {
    return 1;   // Es primo
  }
  return 0;  // NO es primo
}

void factorizer_toString(shared_data_t* sharedData) {
  printf("%"PRId64": ", sharedData->integer);
  int64_t integer = sharedData->integer;
  for (uint64_t index = 0 ;
    index < sharedData->factors.size; index++) {
    int64_t counter = 0;
    int64_t div = sharedData->factors.elements[index];
    while (integer % div == 0) {
      integer = integer/div;
      counter++;
    }
    if (counter > 1) {
      printf("%"PRId64"^" "%"PRId64" ", sharedData->factors.elements[index],
        counter);
    } else {
      printf("%"PRId64 " ", sharedData->factors.elements[index]);
    }
  }
  printf("\n");
}

void factorizer_destroy(factorizer_t* factorizer) {
  assert(factorizer);
  free(factorizer);
}
