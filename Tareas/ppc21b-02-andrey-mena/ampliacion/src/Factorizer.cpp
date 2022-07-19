// Copyright 2022 ECCI-UCR

#include "Factorizer.hpp"

void Factorizer::factorize_brute(FactNumber& number) {
  if (number.state == valid) {
    int64_t copy = this->divide(number, number.number, 2);
    for (int64_t prime = 3; copy > 1; prime += 2) {
      copy = divide(number, copy, prime);
    }
  }
}

int64_t Factorizer::divide(FactNumber& number, int64_t copy, int64_t prime) {
  int64_t times = 0;
  for (; copy % prime == 0; ++times) {
    copy /= prime;
  }
  if (times > 0) {
    number.factors.push_back(prime);
    number.factors.push_back(times);
  }
  return copy;
}

// https://www.geeksforgeeks.org/print-all-prime-factors-of-a-given-number/
void Factorizer::factorize_sqrt(FactNumber& number) {
  if (number.state == valid) {
    // Print powers of two
    int64_t rest = divide(number, number.number, 2), copy = rest;
    int64_t root = isqrt64u(rest);
    // Print powers of odd numbers
    for (int64_t odd = 3; odd <= root; odd += 2) {
      copy = divide(number, rest, odd);
      if (copy < rest) {
        rest = copy;
        root = isqrt64u(rest);
      }
    }
    // If after dividing the number by the first sqrt(n) divisors, there is
    // something left, it is a prime number
    if (copy > 2) {
      number.factors.push_back(copy);
      number.factors.push_back(1);
    }
  }
}

// Square root of integer from https://en.wikipedia.org/wiki/Integer_square_root
uint64_t Factorizer::isqrt64u(uint64_t subradical) {
  uint64_t x0 = subradical >> 1;  // Initial estimate

  if (x0) {  // Sanity check
    uint64_t x1 = (x0 + subradical / x0) >> 1;  // Update

    while (x1 < x0) {  // This also checks for cycle
      x0 = x1;
      x1 = (x0 + subradical / x0) >> 1;
    }

    return x0;
  }

  return subradical;
}
