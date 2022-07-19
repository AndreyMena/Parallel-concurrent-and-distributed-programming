// Copyright 2022 ECCI-UCR

#ifndef FACTORIZER_H
#define FACTORIZER_H

#include <cstdint>

#include "FactCommon.hpp"

class Factorizer {
 public:
  void factorize_brute(FactNumber& number);
  int64_t divide(FactNumber& number, int64_t copy, int64_t prime);
  void factorize_sqrt(FactNumber& number);
  static uint64_t isqrt64u(uint64_t subradical);
};

#endif  // FACTORIZER_H
