// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0
// Serial web server's initial code for parallelization

#ifdef WEBSERVER

#include <csignal>

#include "GoldbachWebApp.hpp"
#include "HttpServer.hpp"

void signal_handler(int numeroSenial);

/// Start the web server
int main(int argc, char* argv[]) {
  signal(SIGINT, signal_handler);
  signal(SIGTERM, signal_handler);
  // Create the web server
  HttpServer& httpServer = HttpServer::getInstance();
  // Create a goldbach web application, and other apps if you want
  GoldbachWebApp goldbachWebApp;
  // Register the web application(s) with the web server
  httpServer.chainWebApp(&goldbachWebApp);
  // Start the web server
  return httpServer.start(argc, argv);
}

void signal_handler(int numeroSenial) {
  numeroSenial = numeroSenial;
  /*
  Log::append(Log::INFO, "Server",
              std::string("Interrumpiendo (" + std::to_string(numeroSenial)) +
                  std::string(")"));*/
  HttpServer::getInstance().stop();
}

#endif  // WEBSERVER
