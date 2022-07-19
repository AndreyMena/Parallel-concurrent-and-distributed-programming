// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef HTTPAPP_H
#define HTTPAPP_H

#include "common.hpp"
#include "Assembler.hpp"
#include "Consumer.hpp"
#include <vector>  //NOLINT
#include "PrivateNode.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include <utility>  //NOLINT
/**
@brief Base class for all web applications that can be registered with the
web server.
*/
class HttpApp : public Assembler<
  std::pair<HttpRequest*, HttpResponse*>, PrivateNode*> {
  /// Web application objects are usually complex. This base class does not
  /// require child classes to allow copying
  DISABLE_COPY(HttpApp);

 public:
  /// @brief Constructor
  HttpApp() = default;

  /// @brief Destructor
  ~HttpApp() = default;

  /// @brief Called by the web server when the web server is started
  virtual void start();

  /// @brief Sobreescribe el metodo de consumer run
  /// Permite empezar a consumir
  /// @return int
  int run() override;

  /// @brief Sobreescribe el metodo del consumer, empieza a procesar los
  /// clientes y sus sockets.
  /// @param solicitud pair<HttpRequest*, HttpResponse*>
  /// @return void
  void consume(std::pair<HttpRequest*, HttpResponse*> solicitud) override;

  /// Handle HTTP requests. @see HttpServer::handleHttpRequest()
  /// @return true If this application handled the request, false otherwise
  /// and another chained application should handle it
  /// @param httpRequest HttpRequest&
  /// @param httpResponse HttpResponse&
  virtual bool handleHttpRequest(HttpRequest& httpRequest,
    HttpResponse& httpResponse) = 0;

  /// @brief Called when the web server stops, in order to allow the web
  /// application clean up and finish as well
  virtual void stop();

  /**
   * @brief Redirige la solicitud del cliente para que puede ser procesada
   * @param solicitud pair<HttpRequest*, HttpResponse*>
   * @return void;
  */
  virtual void redirectClient(std::pair<HttpRequest*, HttpResponse*> solicitud)
    = 0;
};

#endif  // HTTPAPP_H
