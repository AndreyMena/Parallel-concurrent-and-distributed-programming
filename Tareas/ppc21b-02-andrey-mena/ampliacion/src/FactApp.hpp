// Copyright 2022 ECCI-UCR

#ifndef FACTAPP_H
#define FACTAPP_H

#include <string>
#include <vector>

#include "FactDecomposer.hpp"
#include "Factorizer.hpp"

class FactApp {
 private:
  int process_number = -1;
  int process_count = -1;

 public:
  FactApp(int process_number, int process_count);
  void run();
};

#endif  // FACTAPP_H
