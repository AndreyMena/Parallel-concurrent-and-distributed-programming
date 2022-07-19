#include "arrayThreeField_int.h"
#include <assert.h>
#include <stdlib.h>

/** 
 * @copyright [2022] Andrey Esteban Mena Espinoza
*/

int arrayThreeField_int_init(arrayThreeField_int_t* array) {
  assert(array);
  array->capacity = 0;
  array->size = 0;
  array->firstField = NULL;
  array->secondField = NULL;
  array->thirdField = NULL;
  return EXIT_SUCCESS;
}

int arrayThreeField_int_addThree(arrayThreeField_int_t* array,
  int64_t element1, int64_t element2, int64_t element3) {
  assert(array);
  if (array->size == array->capacity) {
    if (arrayThreeField_int_increase_capacity(array) != EXIT_SUCCESS) {
      return EXIT_FAILURE;
    }
  }
  array->firstField[array->size] = element1;
  array->secondField[array->size] = element2;
  array->thirdField[array->size] = element3;

  array->size++;
  return EXIT_SUCCESS;
}

int arrayThreeField_int_increase_capacity(arrayThreeField_int_t* array) {
  size_t new_capacity  = 10 * (array->capacity ? array->capacity : 1);

  int64_t* new_firstFields = (int64_t*)
    realloc(array->firstField, new_capacity * sizeof(int64_t));
  int64_t* new_secondFields = (int64_t*)
    realloc(array->secondField, new_capacity * sizeof(int64_t));
  int64_t* new_thirdFields = (int64_t*)
    realloc(array->thirdField, new_capacity * sizeof(int64_t));

  if (new_firstFields && new_secondFields && new_thirdFields) {
    array->capacity = new_capacity;
    array->firstField = new_firstFields;
    array->secondField = new_secondFields;
    array->thirdField = new_thirdFields;
    return EXIT_SUCCESS;
  } else {
    return EXIT_FAILURE;
  }
}

void arrayThreeField_int_destroy(arrayThreeField_int_t* array) {
  assert(array);
  array->capacity = 0;
  array->size = 0;
  free(array->firstField);
  free(array->secondField);
  free(array->thirdField);
}
