#ifndef GOLDBACHCALC_h
#define GOLDBACHCALC_h

#include <stdint.h>

#include "array_int.h"
#include "arrayThreeField_int.h"

/** 
 * @copyright [2022] Andrey Esteban Mena Espinoza
*/

/**
 * @brief Estructura de datos utilizada por el programa para almacenar
 * la cantidad de sumas, las entradas y los fatores de goldbach.
*/
typedef struct data_structure {
  array_int_t inputsArray;  // All the read numbers
  array_int_t numOfSumsArray;  // The numbers of sums from each number

  // One array for each number, inside three or two factors per position.
  arrayThreeField_int_t* listArrayResult;
} data_structure_t;

/**
 * @brief Calculadora de goldbach utilizada para ejecutar las funciones del
 * programa, no contiene campos.
*/
typedef struct goldbachCalc {
}goldbachCalc_t;


/**
 * @brief Crea y retorna nuestra calculadora de goldbach.
 * @return goldbachCalc_t*
*/
goldbachCalc_t* goldbachCalc_create(void);

/**
 * @brief Crea y retorna nuestra estructura de datos
 * @return data_structure_t*
*/
data_structure_t* data_structure_create(void);

/**
 * @brief Funcion que se encarga de correr y dirigir nuestro programa
 * @param data_structure
 * @return int
*/
int goldbachCalc_run(data_structure_t* data_structure);

/**
 * @brief Ejecuta el calculo de la conjetura fuerte de goldbach.
 * @param number
 * @param data_structure
 * @param index
 * @return int
*/
int goldbachCalc_strongConject(int64_t number,
  data_structure_t* data_structure, uint64_t index);

/**
 * @brief Ejecuta el calculo de la conjetura debil de goldbach.
 * @param number
 * @param data_structure
 * @param index
 * @return int
*/
int goldbachCalc_weakConject(int64_t number,
  data_structure_t* data_structure, uint64_t index);

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
 * @brief Destruye nuestra data_structure_t
 * @param data_structure
 * @return void
*/
void data_structure_destroy(data_structure_t* data_structure);

#endif  // GOLDBACHCALC_h
