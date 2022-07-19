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
  array_int_t numbers;
  array_int_t flags;
  uint64_t threadCount;  // cantidad de hilos pedidos
  char salida[1000][1000];  // Entradas de hasta 1000 lineas y mil caracteres
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
 * @return void
*/
void direct_threads(shared_data_t* sharedData);

/**
 * @brief Subrutina para verificar si el numero es factorizable, sino
 *  guarda el respectivo resultado en la salida.
 * @param privateData_t* privateData
 * @param shared_data_t* sharedData
 * @return int
*/
int couldFactorize(int64_t position, shared_data_t* sharedData,
  int64_t integer);

/**
 * @brief Subrutina para factorizar el numero y guardar su resultado.
 * @param privateData_t* privateData
 * @param shared_data_t* sharedData
 * @return int
*/

int factorizeNumber(int64_t position, shared_data_t* sharedData,
  int64_t integer);

/**
 * @brief Subrutina para encontrar los 
 * @param void* data, un lugar de la memoria
 * @return void*
*/
/*
void* factorize_all_numbers(privateData_t* privateData, shared_data_t* sharedData);
*/
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
