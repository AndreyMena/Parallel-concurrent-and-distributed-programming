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
  array_int_init(&shared_data->processOrder);
  if (!shared_data) {
    fprintf(stderr , "Error, couldn't allocate shared_data");
  }
  return shared_data;
}

int goldbachCalc_processZeroCase(shared_data_t* shared_data,
  int process_number, int process_count) {
  process_number = process_number;
  int error = EXIT_SUCCESS;
  // CASO 2: Tenemos mas entradas que procesos.
  int index = 0;
  // Se envia 1 numero en orden a cada proceso para empezar. Luego que
  // cada proceso tiene 1 trabajo el resto sera repartido dinamicamente
  for (int process = 1; process < process_count; process++) {
    // Puede continuar.
    int couldContinue = 1;
    if (MPI_Send(&couldContinue, 1, MPI_INT, /*target*/ process
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      fprintf(stderr, "could not send message");
    }
    // Indice del numero.
    if (MPI_Send(&index, 1, MPI_INT, /*target*/ process
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      fprintf(stderr, "could not send message");
    }
    // Se envia numero inicial.
    int64_t number = shared_data->inputsArray.elements[process-1];
    if (MPI_Send(&number, 1, MPI_INT64_T, /*target*/ process
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      fprintf(stderr, "could not send message");
    }
    error = array_int_add(&shared_data->processOrder, process);
    index++;
  }

  error = goldbachCalc_mpi_send_dynamic(shared_data, index);
  return error;
}

int goldbachCalc_mpi_send_dynamic(shared_data_t* shared_data, int index) {
  // Repartidcion de trabajo dinamicamente.
  while (index < (int) shared_data->inputsArray.size) {
    int readyForMore = 0;
    MPI_Status status;
    if (MPI_Recv(&readyForMore, 1, MPI_INT, MPI_ANY_SOURCE
      , /*tag*/ 0, MPI_COMM_WORLD, &status) != MPI_SUCCESS ) {
      fprintf(stderr, "could not receive message");
    }
    // El contenido de la recepcion (readyForMore) es irrelevante, se
    // utiliza para saber cual proceso esta libre.
    if (readyForMore == 1) {
      int64_t number = shared_data->inputsArray.elements[index];
      // Puede continuar
      int couldContinue = 1;
      if (MPI_Send(&couldContinue, 1, MPI_INT, /*target*/ status.MPI_SOURCE
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fprintf(stderr, "could not send message");
      }
      // Indice del numero
      if (MPI_Send(&index, 1, MPI_INT, /*target*/ status.MPI_SOURCE
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fprintf(stderr, "could not send message");
      }
      // Numero a consumir
      if (MPI_Send(&number, 1, MPI_INT64_T, /*target*/ status.MPI_SOURCE
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fprintf(stderr, "could not send message");
      }
    }
    array_int_add(&shared_data->processOrder, status.MPI_SOURCE);
    index++;
  }
  return EXIT_SUCCESS;
}

int goldbachCalc_processZeroCaseWithLessInputs(shared_data_t* shared_data,
  int process_number, int process_count) {
  process_number = process_number;
  // CASO 1: Tenemos mas o mismos procesos que entradas.
  // Terminar este caso.
  int index = 0;
  for (int process = 1; process <= (int) shared_data->inputsArray.size;
    process++) {
    int couldContinue = 1;
    if (MPI_Send(&couldContinue, 1, MPI_INT, /*target*/ process
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      fprintf(stderr, "could not send message");
    }
    // Indice del numero.
    if (MPI_Send(&index, 1, MPI_INT, /*target*/ process
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      fprintf(stderr, "could not send message");
    }
    // Se envia numero inicial.
    int64_t number = shared_data->inputsArray.elements[process-1];
    if (MPI_Send(&number, 1, MPI_INT64_T, /*target*/ process
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      fprintf(stderr, "could not send message");
    }
    array_int_add(&shared_data->processOrder, process);
    index++;
  }
  index++;  // Condiciones de parada a los restantes.
  if (index < process_count) {
    for (index = index; index < process_count; index++) {
      int couldContinue = 0;
      if (MPI_Send(&couldContinue, 1, MPI_INT, /*target*/ index
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fprintf(stderr, "could not send message");
      }
    }
  }
  return EXIT_SUCCESS;
}

int goldbachCalc_setProcessData(shared_data_t* shared_data,
  int process_number, int process_count) {
  process_count = process_count;
  int error = EXIT_SUCCESS;
  // Enviamos el size de entradas a todos los procesos, para almacenar espacio.
  if (MPI_Bcast(&shared_data->inputsArray.size, /*count*/ 1, MPI_UNSIGNED,
    /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS ) {
    fprintf(stderr, "could not broadcast value count");
  }
  if (process_number != 0) {
    shared_data->numOfSumsArray.size = shared_data->inputsArray.size;;
    shared_data->numOfSumsArray.elements = (int64_t*)
      calloc(shared_data->inputsArray.size, sizeof(int64_t));
    shared_data->inputsArray.elements = (int64_t*)
      calloc(shared_data->inputsArray.size, sizeof(int64_t));
    shared_data->processOrder.size = shared_data->inputsArray.size;
    shared_data->processOrder.elements = (int64_t*)
      calloc(shared_data->inputsArray.size, sizeof(int64_t));
  }

  shared_data->listArrayResult = (arrayThreeField_int_t*)
    calloc(shared_data->inputsArray.size, sizeof(arrayThreeField_int_t));
  for (uint64_t indexArray = 0; indexArray < shared_data->inputsArray.size;
    indexArray++) {
    error =
      arrayThreeField_int_init(&shared_data->listArrayResult[indexArray]);
  }
  return error;
}

int goldbachCalc_run(shared_data_t* shared_data,
  int process_number, int process_count) {
  int error  = EXIT_SUCCESS;

  error = goldbachCalc_setProcessData(shared_data,
    process_number, process_count);

  // Empieza trabajo con MPI.
  if (process_number == 0) {
    if (process_count-1 >= (int) shared_data->inputsArray.size) {
      error = goldbachCalc_processZeroCaseWithLessInputs(shared_data,
        process_number, process_count);
    } else {
      error = goldbachCalc_processZeroCase(shared_data, process_number,
        process_count);
    }
    // Se envian condiciones de parada:
    error = goldbachCalc_sendStopConditions(shared_data, process_number,
      process_count);
  } else {
    if (process_count-1 >= (int) shared_data->inputsArray.size) {
      for (int process = 1; process <= (int) shared_data->inputsArray.size;
        process++) {
        goldbachCalc_mpi_rec_dynamic(shared_data, process_number,
          process_count);
      }
    } else {
      goldbachCalc_mpi_rec_dynamic(shared_data, process_number,
        process_count);
    }
  }
  return error;
}

int goldbachCalc_sendStopConditions(shared_data_t* shared_data,
  int process_number, int process_count) {
  process_number = process_number;
  if (process_count-1 >= (int) shared_data->inputsArray.size) {
    for (int process = 1; process <= (int) process_count; process++) {
      if (process <= (int) shared_data->inputsArray.size) {
        int readyForMore = 0;
        if (MPI_Recv(&readyForMore, 1, MPI_INT, process
          , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
          fprintf(stderr, "could not receive message");
        }
        int couldContinue = 0;
        if (MPI_Send(&couldContinue, 1, MPI_INT, /*target*/ process
          , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
          fprintf(stderr, "could not send message");
        }
      }
    }
  } else {
    for (int process = 1; process < process_count; process++) {
      int readyForMore = 0;
      if (MPI_Recv(&readyForMore, 1, MPI_INT, process
        , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
        fprintf(stderr, "could not receive message");
      }

      int couldContinue = 0;
      if (MPI_Send(&couldContinue, 1, MPI_INT, /*target*/ process
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fprintf(stderr, "could not send message");
      }
    }
  }
  return EXIT_SUCCESS;
}

int goldbachCalc_mpi_rec_dynamic(shared_data_t* shared_data,
  int process_number,
  int process_count) {
  process_count = process_count;
  process_number = process_number;
  int error  = EXIT_SUCCESS;
  while (1) {
    int64_t number = 0;
    int couldContinue = 0;
    uint64_t index = 0;
    if (MPI_Recv(&couldContinue, 1, MPI_INT, 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fprintf(stderr, "could not receive message");
    }
    // Condicion de parada.
    if (couldContinue == 0) {
      break;
    }
    if (MPI_Recv(&index, 1, MPI_INT64_T, 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fprintf(stderr, "could not receive message");
    }
    if (MPI_Recv(&number, 1, MPI_INT64_T, 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fprintf(stderr, "could not receive message");
    }

    int negative = 0;
    if (number < 0) {
      number = number * - 1;  // To negative
      negative = 1;
    }
    if (number % 2 == 0) {
      // Even number
      error = goldbachCalc_strongConject(number, shared_data, index, negative);
    } else {
      // Odd number
      error = goldbachCalc_weakConject(number, shared_data, index, negative);
    }

    int readyForMore = 1;
    if (MPI_Send(&readyForMore, 1, MPI_INT, /*target*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      fprintf(stderr, "could not send message");
    }
  }

  return error;
}

int goldbachCalc_strongConject(int64_t number,
  shared_data_t* shared_data, uint64_t index, int negative) {
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
      firstPrim, secondPrim, number, index, negative) schedule(dynamic)
    for (int64_t firstPrimeNumber = 2; firstPrimeNumber < number;
      firstPrimeNumber++) {
      int64_t secondPrimeNumber = number-firstPrimeNumber;
      if (firstPrimeNumber <= secondPrimeNumber) {
        if (goldbachCalc_isPrime(firstPrimeNumber) == 1) {
          if (goldbachCalc_isPrime(secondPrimeNumber) == 1) {
            #pragma omp atomic
            ++counterOfGoldbachsSolutions;

            if (negative == 1) {
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
  shared_data_t* shared_data, uint64_t index, int negative) {
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
      firstPrim, secondPrim, thirdPrim, number, maxPrimePosible, index, \
      negative) schedule(dynamic)
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

                if (negative == 1) {
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
