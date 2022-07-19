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

#include "goldbachCalc.h"
#include "array_int.h"
#include "arrayThreeField_int.h"

goldbachCalc_t* goldbachCalc_create(void) {
  goldbachCalc_t* goldbachCalc = (goldbachCalc_t*)
    calloc(1, sizeof(goldbachCalc_t));
  return goldbachCalc;
}

data_structure_t* data_structure_create(void) {
  data_structure_t* data_structure = (data_structure_t*)
    calloc(1, sizeof(data_structure_t));
  array_int_init(&data_structure->inputsArray);
  array_int_init(&data_structure->numOfSumsArray);
  if (!data_structure) {
    printf("Error, couldn't allocate simulation or data_structure");
  }
  return data_structure;
}

int goldbachCalc_run(data_structure_t* data_structure) {
  int error  = EXIT_SUCCESS;
  data_structure->listArrayResult = (arrayThreeField_int_t*)
    malloc(data_structure->inputsArray.size * sizeof(arrayThreeField_int_t));
  for (uint64_t indexArray = 0; indexArray < data_structure->inputsArray.size;
    indexArray++) {
    error =
      arrayThreeField_int_init(&data_structure->listArrayResult[indexArray]);
  }

  for (uint64_t index = 0; index < data_structure->inputsArray.size; index++) {
    int64_t number = data_structure->inputsArray.elements[index];
    if (number < 0) {
      number = number * - 1;  // To negative
    }
    if (number % 2 == 0) {
      // Even number
      error = goldbachCalc_strongConject(number, data_structure, index);
    } else {
      // Odd number
      error = goldbachCalc_weakConject(number, data_structure, index);
    }
  }
  return error;
}

// Adaptado de <https://www.youtube.com/watch?v=ROEnh3ji-Oc
int goldbachCalc_strongConject(int64_t number,
  data_structure_t* data_structure, uint64_t index) {
  int error = EXIT_SUCCESS;
  int64_t counterOfGoldbachsSolutions = 0;
  if (0 <= number && number <= 5) {
    error = arrayThreeField_int_addThree(
      &data_structure->listArrayResult[index], 0, 0, 0);
  } else {
    for (int64_t firstPrimeNumber = 2; firstPrimeNumber < number;
      firstPrimeNumber++) {
      if (goldbachCalc_isPrime(firstPrimeNumber) == 1) {
        int64_t secondPrimeNumber = number-firstPrimeNumber;
        if (goldbachCalc_isPrime(secondPrimeNumber) == 1) {
          if (firstPrimeNumber <= secondPrimeNumber) {
            counterOfGoldbachsSolutions++;
            error = arrayThreeField_int_addThree(
              &data_structure->listArrayResult[index], firstPrimeNumber,
              secondPrimeNumber, 0);
          }
        }
      }
    }
  }
  error = array_int_add(&data_structure->numOfSumsArray,
    counterOfGoldbachsSolutions);
  return error;
}

int goldbachCalc_weakConject(int64_t number,
  data_structure_t* data_structure, uint64_t index) {
  int error = EXIT_SUCCESS;
  int64_t counterOfGoldbachsSolutions = 0;
  if (0 <= number && number <= 5) {
    error = arrayThreeField_int_addThree(
      &data_structure->listArrayResult[index], 0, 0, 0);
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
                  &data_structure->listArrayResult[index],
                  firstPrimeNumber, secondPrimeNumber, thirdPrimeNumber);
              }
            }
          }
        }
      }
    }
  }
  error = array_int_add(&data_structure->numOfSumsArray,
    counterOfGoldbachsSolutions);
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

void data_structure_destroy(data_structure_t* data_structure) {
  assert(data_structure);
  size_t size = data_structure->inputsArray.size;
  array_int_destroy(&data_structure->numOfSumsArray);
  array_int_destroy(&data_structure->inputsArray);
  for (uint64_t indexArray = 0; indexArray < size; indexArray++) {
    arrayThreeField_int_destroy(&data_structure->listArrayResult[indexArray]);
  }
  free(data_structure->listArrayResult);
  free(data_structure);
}
