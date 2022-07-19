// Copyright 2021 Andrés Fallas <andres.fallas@ucr.ac.cr>

#include <vector>
#include "Consumer.hpp"
#include "Producer.hpp"
#include "Node.hpp"
#include "NodePriv.hpp"

#ifndef PRIMEFACTORS_H  // NOLINT
#define PRIMEFACTORS_H

using namespace std;

class FactCalculator : public Consumer<NodePriv*>, public Producer<NodePriv*>
{
private:
    /* data */
    vector<int64_t> factores;
public:
    FactCalculator(/* args */);
    ~FactCalculator();
    int run() override;
    void consume(NodePriv* nodoPriv) override;
    vector<int64_t> primeFactors(int64_t n);
};

/**
 * @brief Subrutina para el cálculo de factores primos
 * @code
 * primeFactors(int64_t n);
 * @endcode
 * @param n valor numerico te tipo int64_t
**/

#endif  // TAREAS_PRIMEFACT_PHTREAD_SRC_PRIMEFACTORS_H_" // NOLINT
