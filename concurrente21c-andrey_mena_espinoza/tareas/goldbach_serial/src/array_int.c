#include "array_int.h"
#include <assert.h>
#include <stdlib.h>

/** 
 * @copyright [2021] Andrey Esteban Mena Espinoza
*/

int array_int_init(array_int_t* array) {
  assert(array);
  array->capacity = 0;
  array->size = 0;
  array->elements = NULL;
  return EXIT_SUCCESS;
}

int array_int_add(array_int_t* array, int64_t element) {
  assert(array);
  if (array->size == array->capacity) {
    if (array_int_increase_capacity(array) != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
  }
  array->elements[array->size++] = element;
  return EXIT_SUCCESS;
}

int array_int_increase_capacity(array_int_t* array) {
  size_t new_capacity  = 10 * (array->capacity ? array->capacity : 1);
  int64_t* new_elements = (int64_t*)
    realloc(array->elements, new_capacity * sizeof(int64_t));
  if (new_elements) {
    array->capacity = new_capacity;
    array->elements = new_elements;
    return EXIT_SUCCESS;
  } else {
    return EXIT_FAILURE;
  }
}

void array_int_destroy(array_int_t* array) {
  assert(array);
  array->capacity = 0;
  array->size = 0;
  free(array->elements);
}
