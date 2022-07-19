// Copyright 2022 ECCI-UCR

#include <mpi.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>

#include "FactDecomposer.hpp"
#include "Factorizer.hpp"

void FactDecomposer::processFile(size_t index, const std::string& filename) {
  // Open file
  std::ifstream file(filename);
  fail_if(!file, "could not open " + filename);

  // Create a data structure for this file
  FactFile* factFile = new FactFile(index, filename);
  fail_if(!factFile, "could not create file data structure");

  // Each line of the file contains a number
  std::string line;
  while (std::getline(file, line)) {
    if (line.length() > 0) {
      try {
        int64_t number = std::stoll(line);
        // TODO(you): decomposer must produce (enqueue) instead of factorizing
        FactNumber validNumber(valid, number);
        Factorizer factorizer;
        factorizer.factorize_sqrt(validNumber);
        std::cout << validNumber << std::endl;
      } catch (const std::exception& exception) {
        // TODO(you): invalid numbers must not be factorized
        FactNumber invalidNumber(nonum, -1);
        std::cout << invalidNumber << std::endl;
      }
    }
  }
}
