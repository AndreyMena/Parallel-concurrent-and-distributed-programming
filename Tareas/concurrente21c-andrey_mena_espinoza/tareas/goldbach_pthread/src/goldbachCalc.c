/** 
 * @copyright [2022] Andrey Esteban Mena Espinoza
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <unistd.h>
#include <pthread.h>
#include <math.h>


#include "goldbachCalc.h"
#include "array_int.h"
#include "arrayThreeField_int.h"

typedef struct private_data {
  uint64_t min;  // all read number
  uint64_t max;  // max index to process from the array
  uint64_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

goldbachCalc_t* goldbachCalc_create(void) {
  goldbachCalc_t* goldbachCalc = (goldbachCalc_t*)
    calloc(1, sizeof(goldbachCalc_t));
  if (!goldbachCalc) {
    fprintf(stderr , "Error, couldn't allocate goldbachCalc");
  }
  return goldbachCalc;
}

shared_data_t* shared_data_create(void) {
  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));
  array_int_init(&shared_data->inputsArray);
  array_int_init(&shared_data->numOfSumsArray);
  if (!shared_data) {
    fprintf(stderr , "Error, couldn't allocate shared_data");
  }
  return shared_data;
}

int goldbachCalc_run(shared_data_t* shared_data) {
  int error  = EXIT_SUCCESS;
  shared_data->listArrayResult = (arrayThreeField_int_t*)
    malloc(shared_data->inputsArray.size * sizeof(arrayThreeField_int_t));
  for (uint64_t indexArray = 0; indexArray < shared_data->inputsArray.size;
    indexArray++) {
    error =
      arrayThreeField_int_init(&shared_data->listArrayResult[indexArray]);
  }
  if (shared_data->inputsArray.size < shared_data->thread_count) {
    shared_data->thread_count = shared_data->inputsArray.size;
  }
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  pthread_t* threads = (pthread_t*)
    malloc(shared_data->thread_count * sizeof(pthread_t));

  error = goldbachCalc_create_threads(shared_data, private_data, threads);
  error = goldbachCalc_join_threads(shared_data, threads);

  free(threads);
  free(private_data);
  return error;
}

int goldbachCalc_create_threads(shared_data_t* shared_data,
  private_data_t* private_data, pthread_t* threads) {
  int error = EXIT_SUCCESS;
  uint64_t jobDivision =  // Integer division
    shared_data->inputsArray.size / shared_data->thread_count;
  uint64_t remaining =  // Module for remaining
    shared_data->inputsArray.size % shared_data->thread_count;

  for (uint64_t threadNumber = 0; threadNumber < shared_data->thread_count;
    threadNumber++) {
    uint64_t minRank = (jobDivision * threadNumber) +
      (threadNumber > remaining ? remaining : threadNumber);
    uint64_t maxRank = (jobDivision * (threadNumber+1)) +
      ((threadNumber+1) > remaining ? remaining : (threadNumber+1));
    // Descomentar para ver el mapeo
    // fprintf(stderr,"El hilo numero " "%"PRIu64 " se encarga de: ["
    //  "%"PRIu64", ""%"PRIu64"[\n",threadNumber,minRank, maxRank);

    private_data[threadNumber].min = minRank;
    private_data[threadNumber].max = maxRank;
    private_data[threadNumber].shared_data = shared_data;
    error = pthread_create(&threads[threadNumber], NULL
      , goldbachCalc_directThreads, &private_data[threadNumber]);
    if (error == EXIT_FAILURE) {
      fprintf(stderr, "Error: Couldn't create thread " "%"PRId64 " \n",
        threadNumber);
      return error;
    }
  }
  return error;
}

int goldbachCalc_join_threads(shared_data_t* shared_data, pthread_t* threads) {
  int error = EXIT_SUCCESS;
  for (uint64_t threadNumber = 0; threadNumber < shared_data->thread_count;
    threadNumber++) {
    error = pthread_join(threads[threadNumber], NULL);
    if (error == EXIT_FAILURE) {
      fprintf(stderr, "Error: Couldn't join thread " "%"PRId64 " \n",
        threadNumber);
      return error;
    }
  }
  return error;
}

void* goldbachCalc_directThreads(void* data) {
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  for (uint64_t index = private_data->min; index < private_data->max;
    index++) {
    int64_t number = shared_data->inputsArray.elements[index];
    if (number < 0) {
      number = number * - 1;  // To negative
    }
    if (number % 2 == 0) {
      // Even number
      goldbachCalc_strongConject(number, shared_data, index);
    } else {
      // Odd number
      goldbachCalc_weakConject(number, shared_data, index);
    }
  }
  return NULL;
}

// Adaptado de <https://www.youtube.com/watch?v=ROEnh3ji-Oc
int goldbachCalc_strongConject(int64_t number,
  shared_data_t* shared_data, uint64_t index) {
  int error = EXIT_SUCCESS;
  int64_t counterOfGoldbachsSolutions = 0;
  if (0 <= number && number <= 5) {
    error = arrayThreeField_int_addThree(
      &shared_data->listArrayResult[index], 0, 0, 0);
  } else {
    for (int64_t firstPrimeNumber = 2; firstPrimeNumber < number;
      firstPrimeNumber++) {
      if (goldbachCalc_isPrime(firstPrimeNumber) == 1) {
        int64_t secondPrimeNumber = number-firstPrimeNumber;
        if (goldbachCalc_isPrime(secondPrimeNumber) == 1) {
          if (firstPrimeNumber <= secondPrimeNumber) {
            counterOfGoldbachsSolutions++;
            error = arrayThreeField_int_addThree(
              &shared_data->listArrayResult[index], firstPrimeNumber,
              secondPrimeNumber, 0);
          }
        }
      }
    }
  }
  shared_data->numOfSumsArray.elements[index] = counterOfGoldbachsSolutions;
  return error;
}

int goldbachCalc_weakConject(int64_t number,
  shared_data_t* shared_data, uint64_t index) {
  int error = EXIT_SUCCESS;
  int64_t counterOfGoldbachsSolutions = 0;
  if (0 <= number && number <= 5) {
    error = arrayThreeField_int_addThree(
      &shared_data->listArrayResult[index], 0, 0, 0);
  } else {
    for (int64_t firstPrimeNumber = 2; firstPrimeNumber < number;
      firstPrimeNumber++) {
      if (goldbachCalc_isPrime(firstPrimeNumber) == 1) {
        for (int64_t secondPrimeNumber = 2; secondPrimeNumber < number;
          secondPrimeNumber++) {
          if (goldbachCalc_isPrime(secondPrimeNumber) == 1) {
            int64_t thirdPrimeNumber = number-
              (firstPrimeNumber+secondPrimeNumber);
            if (goldbachCalc_isPrime(thirdPrimeNumber) == 1) {
              if (firstPrimeNumber <= secondPrimeNumber &&
                secondPrimeNumber <= thirdPrimeNumber) {
                counterOfGoldbachsSolutions++;
                error = arrayThreeField_int_addThree(
                  &shared_data->listArrayResult[index],
                  firstPrimeNumber, secondPrimeNumber, thirdPrimeNumber);
              }
            }
          }
        }
      }
    }
  }
  shared_data->numOfSumsArray.elements[index] = counterOfGoldbachsSolutions;
  return error;
}

int goldbachCalc_isPrime(int64_t number) {
  if (number < 2) {
    return 0;  // it isn't prime
  } else {
    for (int64_t prime = 2; prime <= sqrt(number); prime++) {
      if (number % prime == 0) {
        return 0;  // it isn't prime
      }
    }
  }
  return 1;  // it's prime
}

void goldbachCalc_destroy(goldbachCalc_t* goldbachCalc) {
  free(goldbachCalc);
}

void shared_data_destroy(shared_data_t* shared_data) {
  assert(shared_data);
  size_t size = shared_data->inputsArray.size;
  array_int_destroy(&shared_data->numOfSumsArray);
  array_int_destroy(&shared_data->inputsArray);
  for (uint64_t indexArray = 0; indexArray < size; indexArray++) {
    arrayThreeField_int_destroy(&shared_data->listArrayResult[indexArray]);
  }
  free(shared_data->listArrayResult);
  free(shared_data);
}
