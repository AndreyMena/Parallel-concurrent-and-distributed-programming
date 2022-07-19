#ifndef FACTORIZER_h
#define FACTORIZER_h
#include <stdint.h>
#include "array_int.h"
/** 
 * @copyright [2021] <Andrey Esteban Mena Espinoza>
*/

/**
 * @brief structura de datos de memoria compartida.
*/
typedef struct shared_data {
  array_int_t factors;
  uint64_t threadCount;   // cantidad de hilos pedidos
  int64_t integer;
  pthread_mutex_t can_acces_array;
}shared_data_t;

/**
 * @brief structura de datos de memoria privada.
*/
typedef struct privateData privateData_t;

typedef struct factorizer {
  // array_int_t factors;
}factorizer_t;

/**
 * @brief Constructor, crea un factorizer (factorizador utilizado para factorizar enteros).
 * @details 
 *  @code Como se inicializa:
 *          factorizer_t factorizer;
 *          factorizer_create(&factorizer);
 *  @endcode
 * @param void
 * @return factorizer_t*, un objeto factorizer
*/
factorizer_t* factorizer_create(void);

/**
 * @brief Subrutina que corre el programa
 * @param shared_data_t*, un entero de 64 bits.
 * @return int
*/
int factorizer_run(shared_data_t* sharedData);

/**
 * @brief Subrutina que corre el programa
 * @param threads*, Arreglo de hilos.
 * @param sharedData*, informacion compartida.
 * @param privateData*, informacion privada.
 * @param maxLimit, intervalo maximo a buscar factores.
 * @param jobDivision, division de trabajo.
 * @param minLimit, intervalo minimo a buscar factores.
 * @param firstJobDivision, primera division de trabajo.
 * @return void
*/
void directJobs(pthread_t* threads , shared_data_t* sharedData,
  privateData_t*privateData, int64_t maxLimit, int64_t jobDivision,
    int64_t minLimit, int64_t firstJobDivision);

/**
 * @brief Subrutina para encontrar los factores divisores
 * @param void* data, un lugar de la memoria
 * @return void*
*/
void* factorizer_findFactors(void* data);

/**
 * @brief Subrutina para verificar si un numero es primo
 * @param int64_t divider
 * @return int
*/
int factorizer_isPrime(int64_t divider);

/**
 * @brief Verifica que divider no se encuentre en el arreglo.
 * @param shared_data_t*, int64_t 
 * @return void
*/
int verifyRepetition(shared_data_t* sharedData, int64_t divider);

/**
 * @brief Subrutina imprimir los contenido en la base de datos
 * @param shared_data_t*
 * @return void
*/
void factorizer_toString(shared_data_t* sharedData);

/**
 * @brief Destruye el objeto factorizer.
 * @param factorizer_t*, objeto factorizer a destruir
 * @return void
*/
void factorizer_destroy(factorizer_t* factorizer);

#endif  // FACTORIZER_h
