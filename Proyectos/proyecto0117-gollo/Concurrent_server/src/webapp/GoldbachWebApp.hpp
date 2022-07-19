// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef GOLDBACHWEBAPP_HPP
#define GOLDBACHWEBAPP_HPP

#include <Log.hpp>
#include <regex>  // NOLINT
#include <stdexcept>  // NOLINT
#include <string>  // NOLINT
#include <vector>  //NOLINT
#include <utility>  //NOLINT

#include "HttpApp.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"

class GoldbachCalculate;
class Empaquetador;
class Despachador;

/**
@brief A web application that calculates goldbach sums.
*/

class GoldbachWebApp : public HttpApp {
  /// Objects of this class cannot be copied
  DISABLE_COPY(GoldbachWebApp);

 public:
  /**
   * @brief Constructor
  */
  GoldbachWebApp();
  /**
   * @brief Destructor
  */
  ~GoldbachWebApp();

  /**
   * @brief Called by the web server when the web server is started
   * @return void
  */
  void start();

  /**
   * @brief Called by serveGoldbachSum to read request and store the numbers
   *        in the vector to process
   * @param uri string
   * @param matches smatch
   * @param numbers vector<int64_t>&
   * @return void
  */
  void readRequest(std::string uri, std::smatch matches,
                   std::vector<int64_t>& numbers);

  /**
   * @brief Called by serveGoldbachSum to produce
   * @param httpResponse HttpResponse&
   * @param numbers vector<int64_t>&
   * @return void
  */
  void produceNums(HttpResponse& httpResponse, std::vector<int64_t>& numbers);

  /**
   * Handle HTTP requests. @see HttpServer::handleHttpRequest()
   * @return true If this application handled the request, false otherwise
   * and another chained application should handle it
   * @param httpRequest HttpRequest&
   * @param httpResponse HttpResponse&
  */
  bool handleHttpRequest(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /**
   * @brief Called when the web server stops, in order to allow the web application
   *        clean up and finish as well
   * @return void
  */
  void stop();

  /**
   * @brief Redirects the client to process the request
   * @return void
   * @param solicitud pair<HttpRequest*, HttpResponse*>
   * @return void
  */
  void redirectClient(
      std::pair<HttpRequest*, HttpResponse*> solicitud) override;

  /**
   * @brief Called when the web server stops, in order to allow the web application
   *        clean up and finish as well
   * @return void
   * @param pair<HttpRequest*, HttpResponse*>
   * @return void
  */
  std::vector<GoldbachCalculate*> goldbachCalculators;
  Queue<SharedNode*> empaquetadorColaProduct;

  /**
   * @brief Empaquetador, se encarga de verificar que la solicitud ya haya sido
   * completamente procesada, por lo que verifica que las sumas de goldbach
   * ya este listas.
  */
  Empaquetador* empaquetador;

  /**
   * @brief Despachador, se encarga de ensamblar el mensaje e imprimirselo
   * al usuario.
  */
  Despachador* despachador;

 protected:
  /// Handle HTTP requests. @see HttpServer::handleHttpRequest()
  /// @return true If this application handled the request, false otherwise
  /// Sends the homepage as HTTP response
  /// @param httpRequest HttpRequest&
  /// @param httpResponse HttpResponse&
  bool serveHomepage(HttpRequest& httpRequest, HttpResponse& httpResponse);

  /// Handle a HTTP request that starts with "/goldbach"
  /// @return true if the goldbach sum was handled, false if it must be
  /// handled by another application
  /// @param httpRequest HttpRequest&
  /// @param httpResponse HttpResponse&
  bool serveGoldbachSum(HttpRequest& httpRequest, HttpResponse& httpResponse);
};

#endif  // GOLDBACHWEBAPP_HPP
