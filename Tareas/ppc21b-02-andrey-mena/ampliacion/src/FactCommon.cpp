// Copyright 2022 ECCI-UCR

#include "FactCommon.hpp"

// FactNumber -----------------------------------------------------------------

FactNumber::FactNumber(State state, int64_t number)
  : state(state), number(number) {
  if (this->state == valid) {
    if (this->number < 0) {
      this->state = invalid;
    } else if (number <= 1) {
      this->state = na;
    }
  }
}

std::ostream& FactNumber::printPowers(std::ostream& out) const {
  out << this->number << ':';
  for (size_t index = 0; index < this->factors.size(); index += 2) {
    const int64_t prime = this->factors[index];
    assert(index + 1 < this->factors.size());
    const int64_t times = this->factors[index + 1];

    if (times > 0) {
      out << ' ' << prime;
      if (times > 1) {
        out << '^' << times;
      }
    }
  }
  return out;
}

// FactFile -------------------------------------------------------------------

void FactFile::write() const {
  std::ofstream file(this->filename + ".out");  // e.g: input001.txt.out
  fail_if(!file, "coult not create output file");
  // TODO(you): write numbers and their factorization to the output file
}

// Called to print the report line to the standar output
std::string FactFile::generateReport() const {
  std::ostringstream report;
  report << this->fileIndex << ' '
    << this->filename << ' ';
  return report.str();
}
