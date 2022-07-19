// Copyright 2022 ECCI-UCR

#ifndef FACTCOMMON_H
#define FACTCOMMON_H

#include <cassert>
#include <cinttypes>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#define fail(msg) throw std::runtime_error(msg)
#define fail_if(cond, msg) if (cond) throw std::runtime_error(msg)

enum State {
  unknown,
  nonum,
  invalid,
  na,
  valid,
};

struct FactFile;

struct FactNumber {
  State state;
  int64_t number;
  std::vector<int64_t> factors;
  // TODO(you): Add more fields to trace numbers across the production chain

  explicit FactNumber(State state = unknown, int64_t number = -1);

  inline bool operator==(const FactNumber& other) const {
    return this->state == other.state && number == other.number; }

  friend std::ostream& operator<<(std::ostream& out, const FactNumber& num) {
    switch (num.state) {
      case nonum: return out << "invalid number";
      case invalid: return out << num.number << ": invalid number";
      case na: return out << num.number << ": NA";
      case valid: return num.printPowers(out);
      default: assert(false); return out;
    }
  }

  std::ostream& printPowers(std::ostream& out) const;
};

struct FactFile {
  size_t fileIndex;
  std::string filename;
  // TODO(you): A factorization file must store data about all its numbers

  FactFile(size_t fileIndex, const std::string& filename)
    : fileIndex(fileIndex), filename(filename) { }

  // Write the factorization output
  void write() const;

  // Called to print the report line to the standar output
  std::string generateReport() const;
};

#endif  // FACTCOMMON_H
