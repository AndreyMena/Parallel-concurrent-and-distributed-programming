/** 
 * @copyright [2022] Andrey Esteban Mena Espinoza
*/
#ifndef GOLDBACHCALC_h
#define GOLDBACHCALC_h

#include <stdint.h>
#include <mpi.h>
#include <omp.h>

#include "array_int.h"
#include "arrayThreeField_int.h"


/**
 * @brief Estructura de datos en memoria compartida utilizada por todos 
 * los hilos de ejecucion para almacenar la cantidad de sumas, las
 * entradas y los fatores de goldbach.
*/
typedef struct shared_data {
  array_int_t inputsArray;  // All the read numbers
  array_int_t numOfSumsArray;  // The numbers of sums from each number
  arrayThreeField_int_t* listArrayResult;  // List of factors
  uint64_t thread_count;  // Cantidad de hilos.

  array_int_t processOrder;  // Orden de procesos en indice de impresion
} shared_data_t;

/**
 * @brief Estructura de datos en memoria privada utiizada por cada hilo
 * individual, contiene sus datos necesarios para concretar su trabajo.
*/
typedef struct private_data private_data_t;

/**
 * @brief Calculadora de goldbach utilizada para ejecutar las funciones del
 * programa, no contiene campos.
*/
typedef struct goldbachCalc {
} goldbachCalc_t;

/**
 * @brief Crea y retorna nuestra calculadora de goldbach.
 * @return goldbachCalc_t*
*/
goldbachCalc_t* goldbachCalc_create(void);

/**
 * @brief Crea y retorna nuestra estructura de datos
 * @return shared_data_t*
*/
shared_data_t* shared_data_create(void);

/**
 * @brief Se encarga de enviar el trabajo dinamicamente.
 * @param shared_data shared_data_t*
 * @param index int
 * @return int
*/
int goldbachCalc_mpi_send_dynamic(shared_data_t* shared_data, int index);

/**
 * @brief Setea la shared data de cada proceso.
 * @param shared_data shared_data_t*
 * @param process_count int
 * @param process_number int
 * @return int
*/
int goldbachCalc_setProcessData(shared_data_t* shared_data,
  int process_number, int process_count);

/**
 * @brief Funcion que se encarga de correr y dirigir nuestro programa
 * @param shared_data shared_data_t*
 * @param process_number int
 * @param process_count int
 * @return int
*/
int goldbachCalc_run(shared_data_t* shared_data,
  int process_number, int process_count/*, const char* process_hostname*/);

/**
 * @brief Envia el trabajo de proceso 0 en un caso en el que
 *        la cantidad de entradas es menor o igual que la
 *        cantidad de procesos.
 * @param shared_data shared_data_t*
 * @param process_number int
 * @param process_count int
 * @return int
*/
int goldbachCalc_processZeroCaseWithLessInputs(shared_data_t* shared_data,
  int process_number, int process_count);

/**
 * @brief Envia el trabajo de proceso 0 en un caso normal
 *        (no hay menos entradas que procesos.)
 * @param shared_data shared_data_t*
 * @param process_number int
 * @param process_count int
 * @return int
*/
int goldbachCalc_processZeroCase(shared_data_t* shared_data,
  int process_number, int process_count);

/**
 * @brief Funcion que es ejecutada por los procesos distintos de 
 *        cero en la que reciben el trabajo de forma dinamica
 *        (numero por numero).
 * @param shared_data shared_data_t*
 * @param process_number int
 * @param process_count int
 * @return int
*/
int goldbachCalc_mpi_rec_dynamic(shared_data_t* shared_data,
  int process_number, int process_count);

/**
 * @brief Funcion que es ejecutada por el proceso numero cero
 *        en la que envia las condiciones de parada a el resto
 *        de hilos para que estos terminen de consumir numeros 
 *        dinamicamente y salgan del ciclo.
 * @param shared_data shared_data_t*
 * @param process_number int
 * @param process_count int
 * @return int
*/
int goldbachCalc_sendStopConditions(shared_data_t* shared_data,
  int process_number, int process_count);

/**
 * @brief Ejecuta el calculo de la conjetura fuerte de goldbach.
 * @param number int64_t
 * @param shared_data shared_data_t*
 * @param index uint64_t
 * @param negative int
 * @return int
*/
int goldbachCalc_strongConject(int64_t number,
  shared_data_t* shared_data, uint64_t index, int negative);

/**
 * @brief Ejecuta el calculo de la conjetura debil de goldbach.
 * @param number int64_t
 * @param shared_data shared_data_t*
 * @param index uint64_t
 * @param negative int
 * @return int
*/
int goldbachCalc_weakConject(int64_t number,
  shared_data_t* shared_data, uint64_t index, int negative);

/**
 * @brief Verifica si un numero es primo o no, retornando 1 si lo es y 0 si no.
 * @param number
 * @return int
*/
int goldbachCalc_isPrime(int64_t number);

/**
 * @brief Destruye nuestra goldbachCalc
 * @param goldbachCalc
 * @return void
*/
void goldbachCalc_destroy(goldbachCalc_t* goldbachCalc);

/**
 * @brief Destruye nuestra shared_data_t
 * @param shared_data
 * @return void
*/
void shared_data_destroy(shared_data_t* shared_data);

#endif  // GOLDBACHCALC_h
