// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include <cassert>
#include <stdexcept>
#include <string>

#include "HttpApp.hpp"
#include "HttpRequest.hpp"
#include "HttpResponse.hpp"
#include "HttpServer.hpp"
#include "Log.hpp"
#include "NetworkAddress.hpp"
#include "Queue.hpp"
#include "Socket.hpp"
#include <utility>  // NOLINT
#include "HttpConnectionHandler.hpp"

using namespace std;  // NOLINT

HttpConnectionHandler::HttpConnectionHandler(
    Queue<Socket>* colaSockets,
    Queue<std::pair<HttpRequest*, HttpResponse*>>* requestQue) {
      this->setConsumingQueue(colaSockets);
      this->setProducingQueue(requestQue);
    }
void HttpConnectionHandler::consume(Socket client) {
  this->redirectClient(client);
}

int HttpConnectionHandler::run() {
  this->consumeForever();
  return 0;
}

void HttpConnectionHandler::redirectClient(Socket client) {
  while (true) {
    // Create an object that parses the HTTP request from the socket
    HttpRequest* httpRequest = new HttpRequest(client);

    // If the request is not valid or an error happened
    if (!httpRequest->parse()) {
      delete httpRequest;
      // Non-valid requests are normal after a previous valid request. Do not
      // close the connection yet, because the valid request may take time to
      // be processed. Just stop waiting for more requests
      break;
    }

    // A complete HTTP client request was received. Create an object for the
    // server responds to that client's request
    HttpResponse* httpResponse = new HttpResponse(client);

    // Give subclass a chance to respond the HTTP request
    const bool handled = this->handleHttpRequest(*httpRequest, *httpResponse);

    // If subclass did not handle the request or the client used HTTP/1.0
    if (!handled || httpRequest->getHttpVersion() == "HTTP/1.0") {
      // The socket will not be more used, close the connection
      delete httpRequest;
      client.close();
      break;
    }
    // break;
  }
}

bool HttpConnectionHandler::handleHttpRequest(HttpRequest& httpRequest,
                                              HttpResponse& httpResponse) {
  // Print IP and port from client
  const NetworkAddress& address = httpRequest.getNetworkAddress();
  Log::append(Log::INFO, "connection",
              std::string("connection established with client ") +
                  address.getIP() + " port " +
                  std::to_string(address.getPort()));

  // Print HTTP request
  Log::append(Log::INFO, "request",
              httpRequest.getMethod() + ' ' + httpRequest.getURI() + ' ' +
                  httpRequest.getHttpVersion());
  return this->route(httpRequest, httpResponse);
}

bool HttpConnectionHandler::route(HttpRequest& httpRequest,
                                  HttpResponse& httpResponse) {
  // Traverse the chain of applications
  for (size_t index = 0; index < this->applications.size(); ++index) {
    // If this application handles the request
    HttpApp* app = this->applications[index];
    if (app->handleHttpRequest(httpRequest, httpResponse)) {
      std::pair<HttpRequest*, HttpResponse*> solicitud;
      std::get<0>(solicitud) = &httpRequest;
      std::get<1>(solicitud) = &httpResponse;
      produce(solicitud);
      return true;
    }
  }

  // Unrecognized request
  return this->serveNotFound(httpRequest, httpResponse);
}

bool HttpConnectionHandler::serveNotFound(HttpRequest& httpRequest,
                                          HttpResponse& httpResponse) {
  httpResponse.setStatusCode(404);
  httpResponse.setHeader("Server", "AttoServer v1.0");
  httpResponse.setHeader("Content-type", "text/html; charset=ascii");

  // Build the body of the response
  std::string title = "Not found";
  httpResponse.body()
      << "<!DOCTYPE html>\n"
      << "<html lang=\"en\">\n"
      << "  <meta charset=\"ascii\"/>\n"
      << "  <title>" << title << "</title>\n"
      << "  <style>body {font-family: monospace} h1 {color: red}</style>\n"
      << "  <h1>" << title << "</h1>\n"
      << "  <p>The requested resouce was not found on this server.</p>\n"
      << "  <hr><p><a href=\"/\">Homepage</a></p>\n"
      << "</html>\n";

  // Send the response to the client (user agent)
  int error = httpResponse.send();
  if ( 0 == 1 ) {
    httpRequest.getBodyLength();
  }
  delete &httpResponse;
  return error;
}
