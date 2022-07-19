#ifndef ARRAY_INT_H
#define ARRAY_INT_H
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

/** 
 * @copyright [2021] Andrey Esteban Mena Espinoza
*/

/**
 * @brief Structura de datos de array para enteros.
*/
typedef struct array_int {
  size_t capacity;
  size_t size;
  int64_t* elements;
} array_int_t;

/**
 * @brief Inicializa el arreglo
 * @param array
 * @return int
*/
int array_int_init(array_int_t* array);

/**
 * @brief Agrega un elemento al arreglo
 * @param array
 * @param element
 * @return int
*/
int array_int_add(array_int_t* array, int64_t element);

/**
 * @brief Aumenta la capacidad de l arreglo
 * @param array
 * @return int
*/
int array_int_increase_capacity(array_int_t* array);

/**
 * @brief Destruye el arreglo.
 * @param array
 * @return void
*/
void array_int_destroy(array_int_t* array);
#endif  // ARRAY_INT_H
