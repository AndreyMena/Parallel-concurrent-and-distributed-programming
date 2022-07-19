#pragma once;
// Copyright 2021 Andrés Fallas <andres.fallas@ucr.ac.cr>
#include <vector>
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpApp.hpp"
#include "Node.hpp"

//Node::Node(){}

Node::Node(/* args */ int numbers_amout_c, HttpRequest& httpRequest_c, HttpResponse& httpResponse_c) {
  //this->client = httpResponse_c.getSocket();
  //HttpRequest& httpreq = httpRequest_c;
  //HttpResponse& httpresponse = httpResponse_c;
  
  httpRequest = &httpRequest_c;
  httpResponse = &httpResponse_c;

  numbers_amout = numbers_amout_c;
  numbers_count = 0;
  
  //FALTA INICIALIZAR EL VECTOR DE FACTORES.
  factorizations_array.resize(numbers_amout_c);
  stop = false;
}

Node::~Node()
{
}
