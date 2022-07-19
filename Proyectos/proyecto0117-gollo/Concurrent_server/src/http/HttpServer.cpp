// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include "HttpServer.hpp"

#include <unistd.h>  //NOLINT

#include <cassert>
#include <stdexcept>
#include <string>
#include "PrivateNode.hpp"

#include "HttpApp.hpp"
#include "HttpConnectionHandler.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "Producer.hpp"  // Agregado
#include "Queue.hpp"     // Agregado
#include "Socket.hpp"

const char* const usage =
    "Usage: webserv [port] [handlers]\n"
    "\n"
    "  port        Network port to listen incoming HTTP requests, "
    "default " DEFAULT_PORT
    "\n"
    "  handlers     Number of connection handler theads\n";

HttpServer::HttpServer() {}

HttpServer::~HttpServer() {}

HttpServer& HttpServer::getInstance() {
  static HttpServer servidor;
  return servidor;
}

void HttpServer::listenForever(const char* port) {
  return TcpServer::listenForever(port);
}

void HttpServer::handleClientConnection(Socket& client) {
  this->producingQue.push(client);
}

void HttpServer::chainWebApp(HttpApp* application) {
  assert(application);
  this->applications.push_back(application);
}

int HttpServer::start(int argc, char* argv[]) {
  bool stopApps = false;
  HttpServer();
  try {
    if (this->analyzeArguments(argc, argv)) {
      // Start the log service
      Log::getInstance().start();

      // Start all web applications
      for (size_t index = 0; index < this->applications.size(); ++index) {
        this->applications[index]->setConsumingQueue(&this->requestQue);
        this->applications[index]->setProducingQueue(
          &this->httpAppProducingQueue);
        this->applications[index]->start();
        this->applications[index]->startThread();
      }
      stopApps = true;

      // producingQue = new Queue<Socket>();
      this->http_Conn_Handlers.resize(max_connections);
      for (uint64_t i = 0; i < max_connections; i++) {
        http_Conn_Handlers[i] =
            new HttpConnectionHandler(&this->producingQue, &this->requestQue);
        http_Conn_Handlers[i]->applications = this->applications;
        http_Conn_Handlers[i]->startThread();
      }

      // Start waiting for connections
      this->listenForConnections(this->port);
      const NetworkAddress& address = this->getNetworkAddress();
      Log::append(Log::INFO, "webserver",
                  "Listening on " + address.getIP() + " port " +
                      std::to_string(address.getPort()));

      // Accept all client connections
      this->acceptAllConnections();
    }
  } catch (const std::runtime_error& error) {
    Log::append(Log::INFO, "Programa",
                std::string("Excepcion: no se pueden aceptar mas conexiones"));
    std::cerr << "error: " << error.what() << std::endl;
  }
  for (uint64_t index = 0; index < max_connections; ++index) {
    Socket copia;
    this->producingQue.push(copia);
  }
  for (uint64_t i = 0; i < max_connections; i++) {
    http_Conn_Handlers[i]->waitToFinish();
    if (i == 0) {
      std::pair<HttpRequest*, HttpResponse*> condParada;
      this->requestQue.push(condParada);
    }
    delete http_Conn_Handlers[i];
    /*
    Log::append(Log::INFO, "Thread",
                std::string("Liberando hilo") + std::to_string(i));
    */
  }
  this->http_Conn_Handlers.clear();

  // If applications were started
  if (stopApps) {
    // Stop web applications. Give them an opportunity to clean up
    for (size_t index = 0; index < this->applications.size(); ++index) {
      this->applications[index]->waitToFinish();
      this->applications[index]->stop();
      Log::append(Log::INFO, "App",
                  std::string("Cerrando app ") + std::to_string(index));
    }
  }

  // Stop the log service
  Log::getInstance().stop();

  return EXIT_SUCCESS;
}

bool HttpServer::analyzeArguments(int argc, char* argv[]) {
  // Traverse all arguments
  for (int index = 1; index < argc; ++index) {
    const std::string argument = argv[index];
    if (argument.find("help") != std::string::npos) {
      std::cout << usage;
      return false;
    }
  }

  if (argc >= 2) {
    this->port = argv[1];
  }
  max_connections = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 3) {
    max_connections = atoi(argv[2]);
  }
  return true;
}

void HttpServer::stop() {
  // Stop listening for incoming client connection requests
  HttpServer::getInstance().stopListening();
}
