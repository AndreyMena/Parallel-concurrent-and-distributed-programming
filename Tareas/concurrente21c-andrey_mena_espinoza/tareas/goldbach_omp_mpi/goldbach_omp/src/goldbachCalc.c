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
#include <math.h>
#include <mpi.h>
#include <omp.h>

#include "goldbachCalc.h"
#include "array_int.h"
#include "arrayThreeField_int.h"

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
    calloc(shared_data->inputsArray.size, sizeof(arrayThreeField_int_t));
  for (uint64_t indexArray = 0; indexArray < shared_data->inputsArray.size;
    indexArray++) {
    error =
      arrayThreeField_int_init(&shared_data->listArrayResult[indexArray]);
  }
  for (uint64_t index = 0; index < shared_data->inputsArray.size; index++) {
    int64_t number = shared_data->inputsArray.elements[index];
    if (number < 0) {
      number = number * - 1;  // To negative
    }
    if (number % 2 == 0) {
      // Even number
      error = goldbachCalc_strongConject(number, shared_data, index);
    } else {
      // Odd number
      error = goldbachCalc_weakConject(number, shared_data, index);
    }
  }
  return error;
}

int goldbachCalc_strongConject(int64_t number,
  shared_data_t* shared_data, uint64_t index) {
  int error = EXIT_SUCCESS;
  int64_t counterOfGoldbachsSolutions = 0;
  int64_t* firstPrim = (int64_t*)
    calloc(number, sizeof(int64_t));
  int64_t* secondPrim = (int64_t*)
    calloc(number, sizeof(int64_t));
  int64_t* thirdPrim = (int64_t*)
    calloc(number, sizeof(int64_t));
  if (0 <= number && number <= 5) {
  } else {
    #pragma omp parallel for num_threads(shared_data->thread_count) \
      default(none) shared(shared_data, counterOfGoldbachsSolutions, \
      firstPrim, secondPrim, number, index) schedule(dynamic)
    for (int64_t firstPrimeNumber = 2; firstPrimeNumber < number;
      firstPrimeNumber++) {
      int64_t secondPrimeNumber = number-firstPrimeNumber;
      if (firstPrimeNumber <= secondPrimeNumber) {
        if (goldbachCalc_isPrime(firstPrimeNumber) == 1) {
          if (goldbachCalc_isPrime(secondPrimeNumber) == 1) {
            #pragma omp atomic
            ++counterOfGoldbachsSolutions;

            if (shared_data->inputsArray.elements[index] < 0) {
              int64_t posicion = firstPrimeNumber;
              firstPrim[posicion] = firstPrimeNumber;
              secondPrim[posicion] = secondPrimeNumber;
            }
          }
        }
      }
    }
  }
  shared_data->listArrayResult[index].firstField = firstPrim;
  shared_data->listArrayResult[index].secondField = secondPrim;
  shared_data->listArrayResult[index].thirdField = thirdPrim;
  shared_data->listArrayResult[index].size = number;
  shared_data->numOfSumsArray.elements[index] = counterOfGoldbachsSolutions;
  return error;
}

int goldbachCalc_weakConject(int64_t number,
  shared_data_t* shared_data, uint64_t index) {
  int error = EXIT_SUCCESS;
  int64_t counterOfGoldbachsSolutions = 0;
  int64_t maxPrimePosible = (number * 0.33);
  int64_t maxCapacity = ((number * 0.34) * (number * 0.34));
  int64_t* firstPrim = (int64_t*) calloc(maxCapacity, sizeof(int64_t));
  int64_t* secondPrim = (int64_t*) calloc(maxCapacity, sizeof(int64_t));
  int64_t* thirdPrim = (int64_t*) calloc(maxCapacity, sizeof(int64_t));
  if (0 <= number && number <= 5) {
  } else {
    #pragma omp parallel for num_threads(shared_data->thread_count) \
      default(none) shared(shared_data, counterOfGoldbachsSolutions, \
      firstPrim, secondPrim, thirdPrim, number, maxPrimePosible, index) \
      schedule(dynamic)
    for (int64_t firstPrimeNumber = 2; firstPrimeNumber < number;
      firstPrimeNumber++) {
      if (goldbachCalc_isPrime(firstPrimeNumber) == 1) {
        for (int64_t secondPrimeNumber = 2; secondPrimeNumber < number;
          secondPrimeNumber++) {
          int64_t thirdPrimeNumber = number-
            (firstPrimeNumber+secondPrimeNumber);
          if (firstPrimeNumber <= secondPrimeNumber &&
            secondPrimeNumber <= thirdPrimeNumber) {
            if (goldbachCalc_isPrime(secondPrimeNumber) == 1) {
              if (goldbachCalc_isPrime(thirdPrimeNumber) == 1) {
                #pragma omp atomic
                ++counterOfGoldbachsSolutions;

                if (shared_data->inputsArray.elements[index] < 0) {
                  int64_t posicion = (maxPrimePosible * (firstPrimeNumber-2)) +
                    secondPrimeNumber;
                  firstPrim[posicion] = firstPrimeNumber;
                  secondPrim[posicion] = secondPrimeNumber;
                  thirdPrim[posicion] = thirdPrimeNumber;
                }
              }
            }
          }
        }
      }
    }
  }
  shared_data->listArrayResult[index].firstField = firstPrim;
  shared_data->listArrayResult[index].secondField = secondPrim;
  shared_data->listArrayResult[index].thirdField = thirdPrim;
  shared_data->listArrayResult[index].size = maxCapacity;
  shared_data->numOfSumsArray.elements[index] = counterOfGoldbachsSolutions;
  return error;
}

int goldbachCalc_isPrime(int64_t number) {
  if (number < 2) {
    return 0;  // it isn't prime
  } else {
    if (number == 2) {
      return 1;
    }
    if (number % 2 == 0) {
      return 0;
    }
    double sqrtNumber = sqrt(number);
    for (int64_t prime = 3; prime <= sqrtNumber; prime+=2) {
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
