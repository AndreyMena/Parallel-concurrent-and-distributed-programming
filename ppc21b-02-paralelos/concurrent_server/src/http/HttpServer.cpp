// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <cassert>
#include <stdexcept>
#include <string>
#include <unistd.h>  //NOLINT
#include "HttpApp.hpp"
#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "Socket.hpp"
#include "Producer.hpp"   // Agregado
#include "Queue.hpp"      // Agregado
#include "HttpConnectionHandler.hpp"
#include "FactCalculator.hpp"

const char* const usage =
  "Usage: webserv [port] [handlers]\n"
  "\n"
  "  port        Network port to listen incoming HTTP requests, default "
    DEFAULT_PORT "\n"
  "  handlers     Number of connection handler theads\n";

HttpServer::HttpServer() {
}

HttpServer::~HttpServer() {
}

void HttpServer::listenForever(const char* port) {
  return TcpServer::listenForever(port);
}

void HttpServer::handleClientConnection(Socket& client) {
  Socket copia = client;
  producingQue->push(client);
}

void HttpServer::chainWebApp(HttpApp* application) {
  assert(application);
  this->applications.push_back(application);
}

int HttpServer::start(int argc, char* argv[]) {
  bool stopApps = false;
  try {
    if (this->analyzeArguments(argc, argv)) {
      // Start the log service
      Log::getInstance().start();

      // Start all web applications
      for (size_t index = 0; index < this->applications.size(); ++index) {
        this->applications[index]->start();
      }
      stopApps = true;




      producingQue = new Queue<Socket>();
      this->http_Conn_Handlers.resize(max_connections);
      for (uint64_t i = 0; i < max_connections; i++) {
        http_Conn_Handlers[i] = new HttpConnectionHandler(applications);
        // http_Conn_Handlers[i]->createOwnQueue();
        http_Conn_Handlers[i]->setConsumingQueue(producingQue);
        http_Conn_Handlers[i]->startThread();
      }




      // Start waiting for connections
      this->listenForConnections(this->port);
      const NetworkAddress& address = this->getNetworkAddress();
      Log::append(Log::INFO, "webserver", "Listening on " + address.getIP()
        + " port " + std::to_string(address.getPort()));

      // Accept all client connections
      this->acceptAllConnections();
    }
  } catch (const std::runtime_error& error) {
      //Socket copia;
      //producingQue->push(copia);
      for (uint64_t i = 0; i < max_connections; i++) {
         Socket copia;
         producingQue->push(copia);
      }
      for (uint64_t i = 0; i < max_connections; i++) {
        http_Conn_Handlers[i]->waitToFinish();
        delete http_Conn_Handlers[i];
      }
    std::cerr << "error: " << error.what() << std::endl;
  }

  // If applications were started
  if (stopApps) {
    // Stop web applications. Give them an opportunity to clean up
    for (size_t index = 0; index < this->applications.size(); ++index) {
      this->applications[index]->stop();
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
  this->stopListening();
}
