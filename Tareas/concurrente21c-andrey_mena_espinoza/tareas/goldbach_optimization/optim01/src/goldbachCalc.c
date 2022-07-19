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
  uint64_t index;  // index fron imputsArray
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
  pthread_mutex_init(&shared_data->can_acces_position, NULL);
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
  shared_data->position = shared_data->thread_count - 1;
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
  for (uint64_t threadNumber = 0; threadNumber < shared_data->thread_count;
    threadNumber++) {
    private_data[threadNumber].thread_number = threadNumber;
    private_data[threadNumber].index = threadNumber;
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
  int64_t number = shared_data->inputsArray.elements[private_data->index];
  int finish = 0;
  while (finish == 0) {
    if (number < 0) {
      number = number * - 1;  // To negative
    }
    if (number % 2 == 0) {
      // Even number
      goldbachCalc_strongConject(number, shared_data, private_data->index);
    } else {
      // Odd number
      goldbachCalc_weakConject(number, shared_data, private_data->index);
    }

    pthread_mutex_lock(&shared_data->can_acces_position);
    shared_data->position++;
    private_data->index = shared_data->position;
    if (private_data->index >= shared_data->inputsArray.size) {
      finish = 1;
    } else {
      number = shared_data->inputsArray.elements[private_data->index];
    }
    pthread_mutex_unlock(&shared_data->can_acces_position);
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
  pthread_mutex_destroy(&shared_data->can_acces_position);
  free(shared_data);
}
