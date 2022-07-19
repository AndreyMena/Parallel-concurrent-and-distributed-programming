#ifndef ARRAYTHREEFIELD_INT_H
#define ARRAYTHREEFIELD_INT_H
#include <stddef.h>
#include <stdint.h>
#include <inttypes.h>

/** 
 * @copyright [2022] Andrey Esteban Mena Espinoza
*/

/**
 * @brief Structura de datos de array con tres campos para guardar enteros. 
 * Se utiliza como alternativa para no utilizar 3 vectores distintos, esto
 * debido a que los 3 campos de enteros del array tienen relacion en su
 * respectivo indice. Esta estructura cuenta con los campos: capacidad,
 * size, firstField, secondField y thirdField.
*/
typedef struct arrayThreeField_int {
  size_t capacity;
  size_t size;
  int64_t* firstField;
  int64_t* secondField;
  int64_t* thirdField;
} arrayThreeField_int_t;

/**
 * @brief Inicializa el arreglo
 * @param array
 * @return int
*/
int arrayThreeField_int_init(arrayThreeField_int_t* array);

/**
 * @brief Agrega tres elementos al arreglo.
 * @param array
 * @param element1
 * @param element2
 * @param element3
 * @return int
*/
int arrayThreeField_int_addThree(arrayThreeField_int_t* array,
  int64_t element1, int64_t element2, int64_t element3);

/**
 * @brief Aumenta la capacidad del arreglo
 * @param array
 * @return int
*/
int arrayThreeField_int_increase_capacity(arrayThreeField_int_t* array);

/**
 * @brief Destruye el arreglo.
 * @param array
 * @return void
*/
void arrayThreeField_int_destroy(arrayThreeField_int_t* array);
#endif  // ARRAYTHREEFIELD_INT_H
