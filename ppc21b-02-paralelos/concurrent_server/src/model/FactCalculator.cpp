// Copyright 2021 Andrés Fallas <andres.fallas@ucr.ac.cr>

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <vector>
#include <Node.hpp>

#include "FactCalculator.hpp"
using namespace std;    //NOLINT

// Subrutina para el cálculo de factores primos

FactCalculator::FactCalculator(/* args */) {}
FactCalculator::~FactCalculator() {}

int FactCalculator::run() {
  this->consumeForever();
  return 0;
}

void FactCalculator::consume(NodePriv* nodoPriv) {
    vector<int64_t> factores;
    string prueba;
    int64_t n = nodoPriv->num;
    // Si el número es par, 2 es divisor

    while (n%2 == 0) {
        factores.push_back(2);
        prueba.append(to_string(2));
        n = n / 2;
    }

    // Si el número se encuentra entre 3 y la raíz cuadrada de n

    for (int64_t i = 3; i <= sqrt(n); i = i+2) {
        while (n%i == 0) {
            factores.push_back(i);
            prueba.append(to_string(i));
            n = n/i;
        }
    }

    // Si n es mayor que 2 y es un número primo

    if (n > 2) {
        factores.push_back(n);
        prueba.append(to_string(n));
    }

    
    //sprintf(nodoPriv->nodo->factorizations_array[nodoPriv->index],"%c",prueba); //NOLINT
    nodoPriv->nodo->numbers_count++;
    this->produce(nodoPriv);
    // vector<string> factorizations = nodoPriv->nodo->factorizations_array(); // agregar factores  
    
}
/*

std::vector<int64_t> FactCalculator::primeFactors(int64_t n) {
    vector<int64_t> factores;

    // Si el número es par, 2 es divisor

    while (n%2 == 0) {
        factores.push_back(2);
        n = n / 2;
    }

    // Si el número se encuentra entre 3 y la raíz cuadrada de n

    for (int64_t i = 3; i <= sqrt(n); i = i+2) {
        while (n%i == 0) {
            factores.push_back(i);
            n = n/i;
        }
    }

    // Si n es mayor que 2 y es un número primo

    if (n > 2)
        factores.push_back(n);

    // Retorna un arreglo con los valores factorizados del número n

    return factores;
}
*/