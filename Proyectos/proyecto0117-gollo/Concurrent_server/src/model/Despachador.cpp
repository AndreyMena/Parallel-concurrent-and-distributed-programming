// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include "Despachador.hpp"
#include "HttpResponse.hpp"
#include <string>  // NOLINT

#include "SharedNode.hpp"
#include "PrivateNode.hpp"

Despachador::Despachador(/* args */) { }

bool Despachador::sendResponse(SharedNode* shared_node) {
  return shared_node->httpResponse.send();
}

void Despachador::consume(SharedNode* shared_node) {
  for (size_t index = 0; index <
    shared_node->total_numbers; index++) {
    shared_node->httpResponse.body() << "<br>"
      << "</br>";  // Cambios de linea
    shared_node->httpResponse.body() << "<span>"
      << std::to_string(shared_node->numbers[index]) + ": "
      << "</span>\n";  // \n No influye
    if (shared_node->goldbachsStructures[index].state == 0) {
      shared_node->httpResponse.body() << "<span>"
      << " NA"
      << "</span>";
    } else {
      shared_node->httpResponse.body()
        << "<span>" << std::to_string(
          shared_node->goldbachsStructures[index].numOfSums)
          << "</span>";
      shared_node->httpResponse.body() << "<span>"
        << " sums"
        << "</span>";
    }
    if (shared_node->numbers[index] < 0) {
      int64_t tamanio = shared_node->goldbachsStructures[
          index].firstPrimeNums.size();
      shared_node->httpResponse.body() << "<span>"
        << ": "
        << "</span>";
      for (int64_t indexJ = 0; indexJ < tamanio; indexJ++) {
        int64_t firstNumber = shared_node->goldbachsStructures[
          index].firstPrimeNums[indexJ];
        int64_t secondNumber = shared_node->goldbachsStructures[
          index].secondPrimeNums[indexJ];
        int64_t thirdNumber = shared_node->goldbachsStructures[
          index].thirdPrimeNums[indexJ];
        if (thirdNumber == 0) {
          shared_node->httpResponse.body()
            << "<span>" << std::to_string(firstNumber) + " + "
            << std::to_string(secondNumber) << "</span>";
        } else {
          shared_node->httpResponse.body()
            << "<span>" << std::to_string(firstNumber) + " + "
            << std::to_string(secondNumber) + " + "
            << std::to_string(thirdNumber) << "</span>";
        }
        if (indexJ + 1 != shared_node->goldbachsStructures[
          index].numOfSums) {
          shared_node->httpResponse.body() << "<span>"
            << ", "
            << "</span>";
        }
      }
    }
  }
  shared_node->httpResponse.body()
    << "  <hr><p><a href=\"/\">Back</a></p>\n"
    << "</html>\n";
  this->sendResponse(shared_node);

  pthread_mutex_lock(&shared_node->can_acces_totalNumbers);
  pthread_mutex_unlock(&shared_node->can_acces_totalNumbers);
  pthread_mutex_destroy(&shared_node->can_acces_totalNumbers);
  delete &shared_node->httpResponse;
  delete shared_node;
}
int Despachador::run() {
  this->consumeForever();
  return 0;
}

Despachador::~Despachador() {
}
