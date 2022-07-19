// Copyright 2021 Andrés Fallas <andres.fallas@ucr.ac.cr>

#ifndef NODE_H  // NOLINT
#define NODE_H

#include <stdlib.h>  // NOLINT
#include <vector>
#include <inttypes.h>  // NOLINT
#include <string>  
#include "HttpApp.hpp"

#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

using namespace std;

/**
 * @brief Objeto que contiene los detalles de la
 * solicitud de un cliente
**/

class Node
{
public:
    /* data */
  HttpRequest* httpRequest = nullptr;  // puntero al request
  HttpResponse* httpResponse = nullptr;  // puntero al response
  //Socket client;
  int numbers_amout;  // cantidad de numeros ingresados por el usuario
  int numbers_count;  // contador incializado en 0
  vector<char*> factorizations_array;
  vector<int64_t> factores;  // factores agregados por el cliente RECORDAR
  vector<int64_t> values;
  bool stop; 
public:
  Node(/* args */ int numbers_amout_c, HttpRequest& httpRequest, HttpResponse& httpResponse);
  Node();
  ~Node();
};





#endif  // NOLINT
