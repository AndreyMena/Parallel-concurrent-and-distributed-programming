// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <vector>
#include <utility>

#include "TcpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Producer.hpp"   // Agregado
#include "HttpConnectionHandler.hpp"
#include "PrivateNode.hpp"

#define DEFAULT_PORT "8080"

class HttpApp;

/// TODO: Documentation
class HttpServer : public TcpServer {
  DISABLE_COPY(HttpServer);

 protected:
  /// Lookup criteria for searching network information about this host
  struct addrinfo hints;

  /// TCP port where this web server will listen for connections
  const char* port = DEFAULT_PORT;

  /// Chain of registered web applications. Each time an incoming HTTP request
  /// is received, the request is provided to each application of this chain.
  /// If an application detects the request is for it, the application will
  /// call the httpResponse.send() and the chain stops. If no web app serves
  /// the request, the not found page will be served.
  std::vector<HttpApp*> applications;

  /**
   * Vector de http_Conn_Handlers, consumen las solicitudes, verifican
   * si son aptas y producen en la App.
  */
  std::vector<HttpConnectionHandler*> http_Conn_Handlers;

  /**
   * Cola de produccion para http_Conn_Handlers
  */
  Queue<Socket> producingQue;

  /**
   * Maximo de conneciones admitidas. Determinadas por la cantidad de cpus 
   * que se encuentran disponinbles.
  */
  uint64_t max_connections;

  /**
   * Cola de produccion para los http_Conn_Handlers.
  */
  Queue<std::pair<HttpRequest*, HttpResponse*>> requestQue;

  /**
   * Cola de produccion para las webApp.
  */
  Queue<PrivateNode*> httpAppProducingQueue;

 private:
  /// Constructor
  HttpServer();
  /// Destructor
  ~HttpServer();

 public:
  /// @brief Get access to the unique instance of this Singleton class
  static HttpServer& getInstance();

  /// @brief Registers a web application to the chain
  /// @param application HttpApp*
  void chainWebApp(HttpApp* application);

  /// @brief Start the web server for listening client connections and HTTP
  /// requests
  /// @param argc int
  /// @param argv char*
  /// @return int
  int start(int argc, char* argv[]);

  /// @brief Stop the web server. The server may stop not immediately.
  /// It will stop for listening further connection requests at once,
  /// but pending HTTP requests that are enqueued will be allowed to finish
  /// @return void
  void stop();
  /// @brief Infinetelly listen for client connection requests and accept
  /// all of them. For each accepted connection request, the virtual
  /// onConnectionAccepted() will be called. Inherited classes must
  /// override that method
  /// @param port const char*
  void listenForever(const char* port);

 protected:
  /// @brief Analyze the command line arguments
  /// @return true if program can continue execution, false otherwise
  /// @param argc int
  /// @param argv char**
  bool analyzeArguments(int argc, char* argv[]);

  /// @brief This method is called each time a client connection
  /// request is accepted.
  /// @param client Socket&
  /// @return void
  void handleClientConnection(Socket& client) override;
};

#endif  // HTTPSERVER_H
