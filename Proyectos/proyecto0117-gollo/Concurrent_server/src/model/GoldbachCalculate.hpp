// Copyright 2022 Christopher Obando Salgado,
//                Jose Alexis Valerio Ramirez,
//                Andrey Esteban Mena Espinoza,
//                Adrian Hutt Orozco.
//                Universidad de Costa Rica.

#ifndef GOLDBACHCALCULATE
#define GOLDBACHCALCULATE

#include <cstdint>
#include <cstdlib>
#include <vector>

#include "Assembler.hpp"
#include "GoldbachStruct.hpp"
#include "PrivateNode.hpp"
#include "SharedNode.hpp"
/**
 * @brief Structure for return the proccess values.
 */

class GoldbachCalculate : public Assembler<PrivateNode*, PrivateNode*> {
 public:
  /**
   * @brief GoldbachCalculate constructor
   */
  GoldbachCalculate(/* args */);

  /**
   * @brief GoldbachCalculate destructor
   */
  ~GoldbachCalculate();

  /**
   * @brief Override method from Consumer class
   * @param private_node
   * @return void
   */
  void consume(PrivateNode* private_node) override;

  /**
   * @brief Override method from thread class
   * @param number int64_t
   * @return GoldbachStruct
   */
  int run() override;

  /**
   * @brief Calculates the goldbach's conjeture for odd numbers.
   * @param number int64_t
   * @return GoldbachStruct
   */
  GoldbachStruct goldbachOdd(int64_t number);

  /**
   * @brief Calculates the goldbach's conjeture for even numbers.
   * @param number int64_t
   * @return GoldbachStruct
   */
  GoldbachStruct goldbachEven(int64_t number);

  /**
   * @brief Check if a number is prime or not.
   * @param number int64_t
   * @return int 1 for true and 0 if not
   */
  int isPrime(int64_t number);
};

#endif  // GOLDBACHCALCULATE
