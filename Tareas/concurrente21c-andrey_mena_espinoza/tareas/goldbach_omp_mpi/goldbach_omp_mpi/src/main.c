/** 
 * @copyright [2022] Andrey Esteban Mena Espinoza
*/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <time.h>
#include <mpi.h>
#include <omp.h>

#include "array_int.h"
#include "arrayThreeField_int.h"
#include "goldbachCalc.h"

/**
 * @brief Funcion que se encarga de leer los numeros o archivos por
 * la entrada estandar
 * @param shared_data
 * @return int
*/
int readStdin(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  int64_t number = 0;
  FILE* input = stdin;
  while (!feof(input)) {
    if (fscanf(input, "%"PRId64, &number) == 1) {
      if (!(number > INT64_MIN && number < INT64_MAX)) {
        break;
      }

      error = array_int_add(&shared_data->inputsArray, number);
      error = array_int_add(&shared_data->numOfSumsArray, 0);
    } else {
      break;
    }
  }
  return error;
}

/**
 * @brief Funcion que se encarga del trabajo correspondiente al proceso
 *        cero, en este caso avisar al proceso correspondiente, para
 *        que le envien los datos que necesita recibir para la impresion.
 * @param shared_data shared_data_t*
 * @param process_number int
 * @param process_count int
 * @param readyForSendRec int
 * @param index size_t
 * @return int
*/
int zeroProcessWork(shared_data_t* shared_data, int process_number,
  int process_count, int readyForSendRec, size_t index) {
  process_count = process_count;
  process_number = process_number;
  readyForSendRec = 1;
  int64_t correspondingProcess = shared_data->processOrder.elements[index];
  if (MPI_Send(&readyForSendRec, 1, MPI_INT64_T,  correspondingProcess
    ,  0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fprintf(stderr, "could not send message");
  }
  if (MPI_Recv(&shared_data->numOfSumsArray.elements[index], 1,
    MPI_INT64_T, correspondingProcess
    ,  1, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
    fprintf(stderr, "could not receive message");
  }

  int64_t maxCapacity = -1;
  if (MPI_Recv(&maxCapacity, 1, MPI_INT64_T, correspondingProcess
    ,  2, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
    fprintf(stderr, "could not receive message");
  }
  shared_data->listArrayResult[index].firstField = (int64_t*)
    calloc(maxCapacity, sizeof(int64_t));
  shared_data->listArrayResult[index].secondField = (int64_t*)
    calloc(maxCapacity, sizeof(int64_t));
  shared_data->listArrayResult[index].thirdField = (int64_t*)
    calloc(maxCapacity, sizeof(int64_t));
  if (MPI_Recv(shared_data->listArrayResult[index].firstField,
    maxCapacity, MPI_INT64_T, correspondingProcess
    ,  3, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
    fprintf(stderr, "could not receive message");
  }
  if (MPI_Recv(shared_data->listArrayResult[index].secondField,
    maxCapacity, MPI_INT64_T, correspondingProcess
    ,  4, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
    fprintf(stderr, "could not receive message");
  }
  if (MPI_Recv(shared_data->listArrayResult[index].thirdField,
    maxCapacity, MPI_INT64_T, correspondingProcess
    ,  5, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
    fprintf(stderr, "could not receive message");
  }
  return EXIT_SUCCESS;
}

/**
 * @brief Funcion que se encarga del trabajo correspondiente de todos
 *        los procesos que no son cero, en este caso, reciben el aviso
 *        del hilo 0, y le envian lo que esta solicitando.
 * @param shared_data shared_data_t*
 * @param process_number int
 * @param process_count int
 * @param readyForSendRec int
 * @param index size_t
 * @return int
*/
int noZeroProcessWork(shared_data_t* shared_data, int process_number,
  int process_count, int readyForSendRec, size_t index) {
  process_count = process_count;
  process_number = process_number;
  if (MPI_Recv(&readyForSendRec, 1, MPI_INT64_T, 0
    ,  0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
    fprintf(stderr, "could not receive message");
  }

  // EMPIEZA
  if (MPI_Send(&shared_data->numOfSumsArray.elements[index], 1,
    MPI_INT64_T,  0,  1, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fprintf(stderr, "could not send message");
  }

  // Capacity Tag = 2
  int64_t capacity = shared_data->listArrayResult[index].size;
  if (MPI_Send(&capacity, 1, MPI_INT64_T,  0
    ,  2, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fprintf(stderr, "could not send message");
  }
  // FirstsPrimes Tag = 3
  if (MPI_Send(shared_data->listArrayResult[index].firstField, capacity,
    MPI_INT64_T,  0,  3, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fprintf(stderr, "could not send message");
  }
  // SecondsPrimes Tag = 4
  if (MPI_Send(shared_data->listArrayResult[index].secondField, capacity,
    MPI_INT64_T,  0,  4, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fprintf(stderr, "could not send message");
  }
  // ThirdsPrimes Tag = 5
  if (MPI_Send(shared_data->listArrayResult[index].thirdField, capacity,
    MPI_INT64_T,  0,  5, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fprintf(stderr, "could not send message");
  }
  return EXIT_SUCCESS;
}

/**
 * @brief Funcion que se encarga de dirigir el manejo general de send y rec
 *        entre los procesos 0 y distintos de 0.
 * @param shared_data shared_data_t*
 * @param process_number int
 * @param process_count int
 * @return int
*/
int send_rec_data(shared_data_t* shared_data, int process_number,
  int process_count) {
  // Enviamos el size de entradas a todos los procesos, para almacenar espacio.
  if (MPI_Bcast(shared_data->processOrder.elements,
    shared_data->inputsArray.size, MPI_INT64_T,  0
    , MPI_COMM_WORLD) != MPI_SUCCESS ) {
    fprintf(stderr, "could not broadcast value count");
  }
  for (size_t index = 0; index < shared_data->processOrder.size; index++) {
    // MPI_Status status;  // Por alguna extraña razon si se quita imprime mal.
    // status = status;
    int readyForSendRec = -1;

    if (process_number == 0) {
      zeroProcessWork(shared_data, process_number,
        process_count, readyForSendRec, index);
    }

    if (process_number == shared_data->processOrder.elements[index]) {
      noZeroProcessWork(shared_data, process_number,
        process_count, readyForSendRec, index);
    }
  }
  return EXIT_SUCCESS;
}

/**
 * @brief Funcion que se encarga de imprimir los resultados de goldbach,
 * que se encuentran dentro de la estructura de datos.
 * @param shared_data shared_data_t*
 * @return int
*/
int printResult(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  for (uint64_t indexInput = 0; indexInput < shared_data->inputsArray.size;
    indexInput++) {
    printf("%"PRId64": ", shared_data->inputsArray.elements[indexInput]);
    if (shared_data->numOfSumsArray.elements[indexInput] == 0) {
      printf("NA");
    } else {
      printf("%"PRId64" sums",
        shared_data->numOfSumsArray.elements[indexInput]);
      if (shared_data->inputsArray.elements[indexInput] < 0) {
        printf(": ");
        int contador = 0;
        int64_t indexArr = 0;
        while (contador !=
          shared_data->numOfSumsArray.elements[indexInput]) {
          if (shared_data->listArrayResult[indexInput].firstField[indexArr]
            != 0) {
            if (((shared_data->inputsArray.elements[indexInput] * -1) % 2)
              == 0) {
              printf("%"PRId64" + ",
                shared_data->listArrayResult[indexInput].firstField[
                indexArr]);
              printf("%"PRId64,
                shared_data->listArrayResult[indexInput].secondField[
                indexArr]);
              contador++;
            } else {
              printf("%"PRId64" + ",
                shared_data->listArrayResult[indexInput].firstField[
                  indexArr]);
              printf("%"PRId64" + ",
                shared_data->listArrayResult[indexInput].secondField[
                indexArr]);
              printf("%"PRId64,
                shared_data->listArrayResult[indexInput].thirdField[
                indexArr]);
              contador++;
            }
            if (contador != shared_data->numOfSumsArray.elements[indexInput]) {
              printf(", ");
            }
          }
          indexArr++;
        }
      }
    }
    printf("\n");
  }

  return error;
}

/**
 * @brief Main, redirige el programa
 * @param argc
 * @param argv[]
 * @return int
*/
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int process_number = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);
    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    shared_data_t* shared_data = shared_data_create();
    uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
    if (argc == 2) {
      if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
      } else {
        fprintf(stderr, "Error: invalid thread count\n");
        return EXIT_FAILURE;
      }
    }
    shared_data->thread_count = thread_count;
    error = readStdin(shared_data);
    goldbachCalc_t* goldbachCalc = goldbachCalc_create();

    // Start time.
    struct timespec start, finish;
    clock_gettime(CLOCK_MONOTONIC, &start);

    goldbachCalc_run(shared_data, process_number,
      process_count);  // To measure.

    send_rec_data(shared_data, process_number, process_count);
    if (process_number == 0) {
      printResult(shared_data);

      clock_gettime(CLOCK_MONOTONIC, &finish);
      double duration = finish.tv_sec - start.tv_sec +
        (finish.tv_nsec - start.tv_nsec) / 1e9;
      fprintf(stderr, "Execution time: %.9lf\n", duration);
      // Finish time.
    }
    goldbachCalc_destroy(goldbachCalc);
    shared_data_destroy(shared_data);
    MPI_Finalize();
  } else {
    fprintf(stderr, "error: could not init MPI");
    error = EXIT_FAILURE;
  }
  return error;
}
