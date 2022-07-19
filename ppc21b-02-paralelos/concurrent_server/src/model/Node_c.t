// Copyright 2021 Andrés Fallas <andres.fallas@ucr.ac.cr>

#include "Node.hpp"

class Node {
 public:
  HttpRequest httpRequest;  // puntero al request
  HttpResponse httpResponse;  // puntero al response
  int numbers_amout;  // cantidad de numeros ingresados por el usuario
  int numbers_count;  // contador incializado en 0
  char** factorizations_array;  // arreglo de factorizaciones
  std::vector<int64_t> factores;  // factores agregados por el cliente
  bool stop;

//   Node(HttpRequest& httpRequest, HttpResponse& httpResponse) {
//       numbers_amout = 0;
//       numbers_count = 0;
//       stop = false;
//       }
};
