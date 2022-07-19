// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPCONNECTIONHANDLER_HPP
#define HTTPCONNECTIONHANDLER_HPP

#include <cassert>
#include <stdexcept>
#include <string>
#include <vector>  //NOLINT

#include "Assembler.hpp"
#include "GoldbachCalculate.hpp"
#include "HttpApp.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpServer.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "Queue.hpp"
#include "Socket.hpp"
#include <utility>  // NOLINT

class HttpConnectionHandler
    : public Assembler<Socket, std::pair<HttpRequest*, HttpResponse*>> {
 public:
  std::vector<HttpApp*> applications;
  // Constructor Explicito con parametros
  explicit HttpConnectionHandler(
      Queue<Socket>* colaSockets,
      Queue<std::pair<HttpRequest*, HttpResponse*>>* requestQue);

  // Destructor
  virtual ~HttpConnectionHandler() {}

 protected:
  /// @brief Sobreescribe el metodo de consumer run
  /// Permite empezar a consumir
  /// @return int
  int run() override;

  /// @brief Sobreescribe el metodo del consumer, empieza a procesar los
  ///        clientes y sus sockets.
  /// @param client Socket
  /// @return void
  void consume(Socket client) override;

  /**
   * @brief Redirije el cliente (Socket) para ser procesado.
   * @param client Socket
   * @return void
  */
  void redirectClient(Socket client);

  /// @brief Called each time an HTTP request is received. Web server should
  /// analyze the request object and assemble a response with the response
  /// object. Finally send the response calling the httpResponse.send() method.
  /// @return true on success and the server will continue handling further
  /// HTTP requests, or false if server should stop accepting requests from
  /// this client (e.g: HTTP/1.0)
  /// @param httpRequest HttpRequest&
  /// @param httpResponse HttpResponse&
  bool handleHttpRequest(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
   * @brief Metodo que enruta la solicitud http, esto lo hace una vez ya ha 
   *        sido verificada por lo que produce una unidad hacie la webApp.
   * @param httpRequest HttpRequest&
   * @param httpResponse HttpResponse&
   * @return bool
  */
  bool route(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
   * @brief Es llamado cuando no se encuentra el servidor, error 404.
   * @param httpRequest HttpRequest&
   * @param httpResponse HttpResponse&
   * @return bool
  */
  bool serveNotFound(HttpRequest& httpRequest, HttpResponse& httpResponse);
};

#endif  // HTTPCONNECTIONHANDLER_HPP
