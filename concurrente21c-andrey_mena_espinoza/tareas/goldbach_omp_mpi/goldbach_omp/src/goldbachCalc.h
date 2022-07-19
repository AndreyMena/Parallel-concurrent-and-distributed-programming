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
  arrayThreeField_int_t* listArrayResult;
  uint64_t thread_count;
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
 * @brief Funcion que se encarga de correr y dirigir nuestro programa
 * @param shared_data
 * @return int
*/
int goldbachCalc_run(shared_data_t* shared_data);

/**
 * @brief Ejecuta el calculo de la conjetura fuerte de goldbach.
 * @param number
 * @param shared_data
 * @param index
 * @return int
*/
int goldbachCalc_strongConject(int64_t number,
  shared_data_t* shared_data, uint64_t index);

/**
 * @brief Ejecuta el calculo de la conjetura debil de goldbach.
 * @param number
 * @param shared_data
 * @param index
 * @return int
*/
int goldbachCalc_weakConject(int64_t number,
  shared_data_t* shared_data, uint64_t index);

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
