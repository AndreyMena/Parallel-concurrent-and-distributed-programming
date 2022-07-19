// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
#include "GoldbachWebApp.hpp"

#include <unistd.h>

#include <Log.hpp>
#include <algorithm>  // NOLINT
#include <cassert>  // NOLINT
#include <iostream>  // NOLINT
#include <regex>  // NOLINT
#include <stdexcept>  // NOLINT
#include <string>  // NOLINT
#include <utility>  // NOLINT

#include "Despachador.hpp"
#include "Empaquetador.hpp"
#include "GoldbachCalculate.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "PrivateNode.hpp"
#include "SharedNode.hpp"

GoldbachWebApp::GoldbachWebApp() {}

GoldbachWebApp::~GoldbachWebApp() {}

void GoldbachWebApp::start() {
  // TODO(you): Start producers, consumers, assemblers...

  empaquetador = new Empaquetador();
  empaquetador->createOwnQueue();
  empaquetador->setProducingQueue(&empaquetadorColaProduct);
  empaquetador->startThread();

  despachador = new Despachador();
  despachador->setConsumingQueue(this->empaquetador->getProducingQueue());
  despachador->startThread();

  goldbachCalculators.resize(sysconf(_SC_NPROCESSORS_ONLN));
  for (int index = 0; index < sysconf(_SC_NPROCESSORS_ONLN); index++) {
    goldbachCalculators[index] = new GoldbachCalculate();
    goldbachCalculators[index]->setConsumingQueue(this->getProducingQueue());
    goldbachCalculators[index]->setProducingQueue(
        empaquetador->getConsumingQueue());
    goldbachCalculators[index]->startThread();
  }
}

void GoldbachWebApp::stop() {
  // TODO(you): Stop producers, consumers, assemblers...
  int totalcpu = sysconf(_SC_NPROCESSORS_ONLN);

  for (int index = 0; index < totalcpu; index++) {
    PrivateNode* condParada =
        nullptr;  // Condiciones de parada para calculators
    goldbachCalculators[index]->getConsumingQueue()->push(condParada);
  }

  for (int index = 0; index < totalcpu; index++) {
    PrivateNode* conParadaShared =
        nullptr;  // Condiciones de parada para empaquetador
    if (index == 0) {
      goldbachCalculators[0]->produce(conParadaShared);
    }
  }

  SharedNode* condParadaDespachador =
      nullptr;  // Condiciones de parada para Despachador
  this->empaquetador->getProducingQueue()->push(condParadaDespachador);
  this->empaquetador->waitToFinish();

  for (int index = 0; index < totalcpu; index++) {
    // std::cout<< "hilo: "<<index<<"\n";
    goldbachCalculators[index]->waitToFinish();

    delete goldbachCalculators[index];
  }

  despachador->waitToFinish();
  delete despachador;
  delete empaquetador;
}

void GoldbachWebApp::redirectClient(
    std::pair<HttpRequest*, HttpResponse*> solicitud) {
  this->serveGoldbachSum(*std::get<0>(solicitud), *std::get<1>(solicitud));
}

bool GoldbachWebApp::handleHttpRequest(HttpRequest& httpRequest,
                                       HttpResponse& httpResponse) {
  // If the home page was asked
  if (httpRequest.getMethod() == "GET" && httpRequest.getURI() == "/") {
    return this->serveHomepage(httpRequest, httpResponse);
  }

  // If the request starts with "goldbach/" is for this web app
  if (httpRequest.getURI().rfind("/goldbach", 0) == 0) {
    return true;
    // return this->serveGoldbachSum(httpRequest, httpResponse);
  }

  // Unrecognized request
  return false;
}

// TODO(you): Fix code redundancy in the following methods

bool GoldbachWebApp::serveHomepage(HttpRequest& httpRequest,
                                   HttpResponse& httpResponse) {
  (void)httpRequest;

  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.1");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Goldbach sums";
  httpResponse.body()
      << "<!DOCTYPE html>\n"
      << "<html lang=\"en\">\n"
      << "  <meta charset=\"ascii\"/>\n"
      << "  <title>" << title << "</title>\n"
      << "  <style>body {font-family: monospace}</style>\n"
      << "  <h1>" << title << "</h1>\n"
      << "  <form method=\"get\" action=\"/goldbach\">\n"
      << "    <label for=\"number\">Number</label>\n"
      << "    <input type=\"number\" name=\"number\" required/>\n"
      << "    <button type=\"submit\">GoldbachSum</button>\n"
      << "  </form>\n"
      << "</html>\n";

  // Send the response to the client (user agent)
  return httpResponse.send();
}

void GoldbachWebApp::readRequest(std::string uri, std::smatch matches,
                                 std::vector<int64_t>& numbers) {
  std::stringstream log;

  // Se lee el URI y se seleccionan los numeros unicamente

  const std::regex re("([0-z]+)|(-?\\d+)");
  std::sregex_iterator end;
  std::sregex_iterator iter(uri.begin() + matches.position(2), uri.end(), re);
  int64_t num;

  // Se ingresan los numeros separados por coma a un vector de numeros

  while (iter != end) {
    try {
      num = std::stoll((*iter)[0].str());
      numbers.push_back(num);
      log << "Match"
          << ": " << (*iter)[0].str();
      Log::append(Log::DEBUG, "Goldbach number", log.str());
      log.str("");
    } catch (...) {
    }
    ++iter;
  }
}

void GoldbachWebApp::produceNums(HttpResponse& httpResponse,
                                  std::vector<int64_t>& numbers) {
  GoldbachCalculate GoldbachCalculate;
  std::string title = "Goldbach Sums";
  httpResponse.body() << "  <h1>" << title << "</h1>\n";

  SharedNode* shared_node = new SharedNode(httpResponse);
  shared_node->procces_numbers = 0;
  shared_node->total_numbers = numbers.size();
  shared_node->httpResponse = httpResponse;
  shared_node->goldbachsStructures.resize(numbers.size());
  pthread_mutex_init(&shared_node->can_acces_totalNumbers, NULL);

  shared_node->numbers.resize(numbers.size());
  // Para todos los elementos en el vector, se imprimen las sumas de goldbach
  for (std::size_t i = 0; i < numbers.size(); i++) {
    PrivateNode* private_node = new PrivateNode();
    int64_t number = numbers[i];
    shared_node->numbers[i] = number;

    private_node->number = number;
    private_node->shared_node = shared_node;
    private_node->index = i;
    if (number % 2 == 0) {
      this->produce(private_node);
    } else {
      this->produce(private_node);
    }
  }
}

bool GoldbachWebApp::serveGoldbachSum(HttpRequest& httpRequest,
                                      HttpResponse& httpResponse) {
  (void)httpRequest;
  // Set HTTP response metadata (headers)
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");
  // If a number was asked in the form "/goldbach/1223"
  // or "/goldbach?number=1223"
  // TODO(you): URI can be a multi-value list, e.g: 100,2784,-53,200771728
  // TODO(you): Use arbitrary precision for numbers larger than int64_t
  // TODO(you): Modularize this method
  std::smatch matches;
  std::regex inQuery("^/goldbach(/|\\?number=)(.*)$");
  std::string uri = httpRequest.getURI();
  std::regex decode("(%2C)|(\\+)|(%20)");
  uri = regex_replace(uri, decode, ",");
  if (std::regex_search(uri, matches, inQuery)) {
    assert(matches.length() >= 3);
    std::vector<int64_t> numbers;  // NOLINT include declarado en hpp

    readRequest(uri, matches, numbers);

    produceNums(httpResponse, numbers);
  } else {
    // Build the body for an invalid request
    std::string title = "Invalid request";
    httpResponse.body()
        << "<!DOCTYPE html>\n"
        << "<html lang=\"en\">\n"
        << "  <meta charset=\"ascii\"/>\n"
        << "  <title>" << title << "</title>\n"
        << "  <style>body {font-family: monospace} .err {color: red}</style>\n"
        << "  <h1 class=\"err\">" << title << "</h1>\n"
        << "  <p>Invalid request for goldbach sums</p>\n"
        << "  <hr><p><a href=\"/\">Back</a></p>\n"
        << "</html>\n";
  }
  delete &httpRequest;
  return 0;
}
