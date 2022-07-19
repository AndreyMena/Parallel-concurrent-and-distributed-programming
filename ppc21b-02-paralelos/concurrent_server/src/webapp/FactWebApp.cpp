// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <stdexcept>
#include <string>
#include <vector>
#include <unistd.h>
#include <Node.hpp>
#include <NodePriv.hpp>

#include "FactWebApp.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "FactCalculator.hpp"



FactWebApp::FactWebApp() {
}

FactWebApp::~FactWebApp() {
}

void FactWebApp::start() {
  // TODO(you): Start producers, consumers, assemblers...
  max_connections = sysconf(_SC_NPROCESSORS_ONLN);

  producingQue = new Queue<NodePriv*>();

  //No se logro asignar producinqueue
  this->setProducingQueue(new Queue<NodePriv>());

  this->factCalculators.resize(max_connections);

  // TODO(): inicializar NodePriv

  for (uint64_t i = 0; i < max_connections; i++) {
    factCalculators[i] = new FactCalculator();

    //No se consigue setear ambas colas entre producer y consumer
    // factCalculators[i]->setConsumingQueue(producingQue); // crear queue de solicitudes HTTP
    factCalculators[i]->createOwnQueue();
    factCalculators[i]->setProducingQueue(producingQue); // Mpde
    factCalculators[i]->startThread();
  }

}

void FactWebApp::stop() {
  // TODO(you): Stop producers, consumers, assemblers...
}

bool FactWebApp::handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) {
  // If the home page was asked
  if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/") {
    return this->serveHomepage(httpRequest, httpResponse);
  }

  // If the request starts with "fact/" is for this web app
  if (httpRequest.getURI().rfind("/fact", 0) == 0) {
    return this->serveFactorization(httpRequest, httpResponse);
  }

  // Unrecognized request
  return false;
}

// TODO(you): Fix code redundancy in the following methods
// NodePriv nodoPriv();


bool FactWebApp::serveHomepage(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.1");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Prime factorization";
  httpResponse.body() << "<!DOCTYPE html>\n"
    << "<html lang=\"en\">\n"
    << "  <meta charset=\"ascii\"/>\n"
    << "  <title>" << title << "</title>\n"
    << "  <style>body {font-family: monospace}</style>\n"
    << "  <h1>" << title << "</h1>\n"
    << "  <form method=\"get\" action=\"/fact\">\n"
    << "    <label for=\"number\">Number</label>\n"
    << "    <input type=\"number\" name=\"number\" required/>\n"
    << "    <button type=\"submit\">Factorize</button>\n"
    << "  </form>\n"
    << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}

bool FactWebApp::serveFactorization(HttpRequest& httpRequest
  , HttpResponse& httpResponse) {
  (void)httpRequest;

  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");
  // If a number was asked in the form "/fact/1223"
  // TODO(you): Modularize this method
  std::smatch matches;
  // std::regex inQuery("^/fact(/|\\?number=)(\\d+)$");
  std::regex inQuery(
    "^/fact(/|\\?number=)([1-9]{1}[0-9]{0,9})+((,[1-9]{1}[0-9]{0,9}){0,1})+$");

  if (std::regex_search(httpRequest.getURI(), matches, inQuery)) {
    assert(matches.length() >= 3);
    std::vector<int64_t> factors;
    std::vector<int64_t> numbers;

    // Se lee el URI y se seleccionan los numeros unicamente
    std::string str2 = httpRequest.getURI();
    std::string token = "/fact?number=";
    str2.erase(0, str2.find_first_not_of(token));
    std::stringstream string_uri(str2);

    // Se ingresan los numeros separados por coma a un vector de numeros
    while (string_uri.good()) {
      std::string substr;
      getline(string_uri, substr, ',');
      int64_t value = std::stoll(substr);
      numbers.push_back(value);
    }




    // TODO(afm): Node(HttpRequest& httpRequest, HttpResponse& httpResponse)
    // Node es el homologo a NetworkMessage de simulation
    // TODO(afm): node.numbers_amout == numbers.size()
    // send factorization queue to factCalculator
    // dispacher -> print output
    Node nodo(numbers.size(), httpRequest, httpResponse);
    nodo.values = numbers;
    NodePriv nodoPriv;
    for (int index = 0; index < nodo.numbers_amout; index++) {
      nodoPriv.index = index;
      nodoPriv.nodo = &nodo;
      nodoPriv.num = nodo.values[index];
      this->produce(nodoPriv);
    }
    
    if (nodo.numbers_amout == nodo.numbers_count) {
      //IMPRIME

    } else {

    }
    std::string title = "Prime factorization";
    httpResponse.body() << "  <h1>" << title << "</h1>\n";

    // // Para todos los elementos en el vector, se imprimen las factorizaciones
    // for (std::size_t i = 0; i < numbers.size(); i++) {
    //   int64_t number = numbers[i];
    //   httpResponse.body() << "<br>" << " " << "</br>";
    //   httpResponse.body() << "<span>" <<
    //     std::to_string(number) + " = " << "</span>\n";
    //   std::vector<int64_t> factors_i;
    //   factors_i = primeFactors(number);
    //   for (std::size_t i =0 ; i < factors_i.size(); i++) {
    //     int64_t factor = factors_i[i];
    //     httpResponse.body() << "<span>" <<
    //       std::to_string(factor) + " " << "</span>";
    //   }
    // }

  } else {
    // Build the body for an invalid request
    std::string title = "Invalid request";
    httpResponse.body() << "<!DOCTYPE html>\n"
      << "<html lang=\"en\">\n"
      << "  <meta charset=\"ascii\"/>\n"
      << "  <title>" << title << "</title>\n"
      << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
      << "  <h1 class=\"err\">" << title << "</h1>\n"
      << "  <p>Invalid request for factorization</p>\n"
      << "  <hr><p><a href=\"/\">Back</a></p>\n"
      << "</html>\n";
  }

  // Send the response to the client (user agent)
  return httpResponse.send();
}

int FactWebApp::run() {
  return 0;
}