#ifndef FACTORIZER_h
#define FACTORIZER_h
#include <stdint.h>
/** 
 * @copyright [2021] <Andrey Esteban Mena Espinoza>
*/

/**
 * @brief Database de nuestro factorizer
*/
typedef struct dataBase factorizer_t;

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
 * @brief Subrutina factorize, factoriza un numero entero.
 * @param int64_t, un entero de 64 bits.
 * @return void
*/
void factorizer_factorize(int64_t integer);

/**
 * @brief Destruye el objeto factorizer.
 * @param factorizer_t*, objeto factorizer a destruir
 * @return void
*/
void factorizer_destroy(factorizer_t* factorizer);

#endif  // FACTORIZER_h
