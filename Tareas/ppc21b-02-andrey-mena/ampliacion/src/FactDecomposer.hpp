// Copyright 2022 ECCI-UCR

#ifndef FACTDECOMPOSER_H
#define FACTDECOMPOSER_H

#include <string>

#include "FactCommon.hpp"

class FactDecomposer {
 public:
  void processFile(size_t index, const std::string& filename);
};

#endif  // FACTDECOMPOSER_H
