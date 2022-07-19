// Copyright 2022 Christopher Obando Salgado,
//                Jose Alexis Valerio Ramirez,
//                Andrey Esteban Mena Espinoza,
//                Adrian Hutt Orozco.
//                Universidad de Costa Rica.
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <vector>

#include "Empaquetador.hpp"
#include "GoldbachCalculate.hpp"
#include "GoldbachStruct.hpp"
#include "PrivateNode.hpp"

GoldbachCalculate::GoldbachCalculate(/* args */) {}
GoldbachCalculate::~GoldbachCalculate() {}

void GoldbachCalculate::consume(PrivateNode* private_node) {
  if (private_node->number % 2 == 0) {
    private_node->shared_node->goldbachsStructures[private_node->index] =
        this->goldbachEven(private_node->number);
  } else {
    private_node->shared_node->goldbachsStructures[private_node->index] =
        this->goldbachOdd(private_node->number);
  }
  this->produce(private_node);
}

int GoldbachCalculate::run() {
  this->consumeForever();
  return 0;
}

GoldbachStruct GoldbachCalculate::goldbachOdd(int64_t number) {
  int64_t counterOfGoldbachsSolutions = 0;
  GoldbachStruct goldbachStruct;
  goldbachStruct.state = 1;
  if (number < 0) {
    goldbachStruct.state = -1;
    number = number * -1;
  }
  if (0 <= number && number <= 5) {
    goldbachStruct.state = 0;
  } else {
    for (int64_t firstPrimeNumber = 2; firstPrimeNumber < number;
         firstPrimeNumber++) {
      if (isPrime(firstPrimeNumber) == 1) {
        for (int64_t secondPrimeNumber = 2; secondPrimeNumber < number;
             secondPrimeNumber++) {
          if (isPrime(secondPrimeNumber) == 1) {
            int64_t thirdPrimeNumber =
                number - (firstPrimeNumber + secondPrimeNumber);
            if (isPrime(thirdPrimeNumber) == 1) {
              if (firstPrimeNumber <= secondPrimeNumber &&
                  secondPrimeNumber <= thirdPrimeNumber) {
                counterOfGoldbachsSolutions++;
                goldbachStruct.firstPrimeNums.push_back(firstPrimeNumber);
                goldbachStruct.secondPrimeNums.push_back(secondPrimeNumber);
                goldbachStruct.thirdPrimeNums.push_back(thirdPrimeNumber);
              }
            }
          }
        }
      }
    }
  }
  goldbachStruct.numOfSums = counterOfGoldbachsSolutions;
  return goldbachStruct;
}

GoldbachStruct GoldbachCalculate::goldbachEven(int64_t number) {
  int64_t counterOfGoldbachsSolutions = 0;
  GoldbachStruct goldbachStruct;
  goldbachStruct.state = 1;
  if (number < 0) {
    goldbachStruct.state = -1;
    number = number * -1;
  }
  if (0 <= number && number <= 5) {
    goldbachStruct.state = 0;
  } else {
    for (int64_t firstPrimeNumber = 2; firstPrimeNumber < number;
         firstPrimeNumber++) {
      if (isPrime(firstPrimeNumber) == 1) {
        int64_t secondPrimeNumber = number - firstPrimeNumber;
        if (isPrime(secondPrimeNumber) == 1) {
          if (firstPrimeNumber <= secondPrimeNumber) {
            counterOfGoldbachsSolutions++;
            goldbachStruct.firstPrimeNums.push_back(firstPrimeNumber);
            goldbachStruct.secondPrimeNums.push_back(secondPrimeNumber);
            goldbachStruct.thirdPrimeNums.push_back(0);
          }
        }
      }
    }
  }
  goldbachStruct.numOfSums = counterOfGoldbachsSolutions;
  return goldbachStruct;
}

int GoldbachCalculate::isPrime(int64_t number) {
  /**
   * prime Vale 1 o 0 dependiento si el numero es primo o no
   */
  int prime = 1;
  /**
   * Inicia en una variable auxiliar que empieza en 2 y mientras la raiz
   * cuadrada del numero recibido sea mayor o igual al auxiliar saca el modulo
   * del numero entre el auxiliar y si da 0 significa que el divisible por ese
   * numero y por lo tanto no es primo, por lo que prime se vuelve 0 y sale del
   * ciclo pues ya demostro que no es primo
   */
  for (int64_t aux = 2; aux <= sqrt(number); aux++) {
    if ((number % aux) == 0) {
      prime = 0;
      break;
    }
  }
  /**
   * Retorna el valor de prime
   */
  return prime;
}
