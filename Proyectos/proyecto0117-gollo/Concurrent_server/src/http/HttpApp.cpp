// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include "HttpApp.hpp"
#include <utility>

void HttpApp::start() {
  // Default base class implementation does nothing
}

void HttpApp::stop() {
  // Default base class implementation does nothing
}

void HttpApp::consume(std::pair<HttpRequest*, HttpResponse*> solicitud) {
  this->redirectClient(solicitud);
}

int HttpApp::run() {
  this->consumeForever();
  return 0;
}
