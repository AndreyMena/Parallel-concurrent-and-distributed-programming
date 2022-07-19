// Copyright 2022 Christopher Obando Salgado,
//                Jose Alexis Valerio Ramirez,
//                Andrey Esteban Mena Espinoza,
//                Adrian Hutt Orozco.
//                Universidad de Costa Rica.

#ifndef GOLDBACHSTRUCT_HPP
#define GOLDBACHSTRUCT_HPP

#include <cstdint>
#include <vector>
/**
 * @brief The message to be transmitted by the simulated network
 */
struct GoldbachStruct {
 public:
  /**
   * @brief Vector correspondiente la los primeros factores primos
   *        de la suma de goldbach correspondiente
  */
  std::vector<int64_t> firstPrimeNums;

  /**
   * @brief Vector correspondiente la los segundo factores primos
   *        de la suma de goldbach correspondiente
  */
  std::vector<int64_t> secondPrimeNums;

  /**
   * @brief Vector correspondiente la los terceros factores primos
   *        de la suma de goldbach correspondiente
  */  
  std::vector<int64_t> thirdPrimeNums;

  /**
   * @brief Entero que corresponde a la cantidad de sumas de golbach
   *        que tiene cada numero.
  */  
  int64_t numOfSums;

  /**
   * @brief Estado que corresponde si un numero es negativo
   *        positivo o no se encuentra en el rango correspondido.
  */    
  int state;

 public:
  /// Convenience constructor
  GoldbachStruct() {
  }

  /// @brief Operator==
  /// @param other GoldbachStruct&
  /// @return true if this message is equals to the given
  inline bool operator==(const GoldbachStruct& other) const {
    return this->firstPrimeNums == other.firstPrimeNums
      && this->numOfSums == other.numOfSums
      && this->secondPrimeNums== other.secondPrimeNums
      && this->state== other.state
      && this->thirdPrimeNums== other.thirdPrimeNums;
  }
};

#endif  // GOLDBACHSTRUCT_HPP
