// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPCONNECTIONHANDLER_HPP
#define HTTPCONNECTIONHANDLER_HPP

#include <cassert>
#include <stdexcept>
#include <string>

#include "HttpApp.hpp"
#include "HttpServer.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "Socket.hpp"
#include "Queue.hpp"
#include "Consumer.hpp"
#include <vector>   //NOLINT
#include "FactWebApp.hpp"

class HttpConnectionHandler : public Consumer<Socket>, public Producer<HttpRequest>  {
 public:
  std::vector<HttpApp*> applications;
  explicit HttpConnectionHandler(std::vector<HttpApp*> applicationss);
  ~HttpConnectionHandler();
 protected:
  /// Sobreescribe el metodo de consumer run
  /// Permite empezar a consumir
  int run() override;
  /// Sobreescribe el metodo del consumer, empieza a procesar los
  /// clientes y sus sockets.
  void consume(Socket client) override;
  /// Called each time an HTTP request is received. Web server should analyze
  /// the request object and assemble a response with the response object.
  /// Finally send the response calling the httpResponse.send() method.
  /// @return true on success and the server will continue handling further
  /// HTTP requests, or false if server should stop accepting requests from
  /// this client (e.g: HTTP/1.0)
  bool handleHttpRequest(HttpRequest& httpRequest,
  HttpResponse& httpResponse);
  bool route(HttpRequest& httpRequest, HttpResponse& httpResponse);
  /// Es llamado cuando no se encuentra el servidor, error 404.
  bool serveNotFound(HttpRequest& httpRequest, HttpResponse& httpResponse);
};

#endif  // HTTPCONNECTIONHANDLER_HPP
