// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef DESPACHADOR_HPP
#define DESPACHADOR_HPP
#include "HttpResponse.hpp"
#include <cstdint>  // NOLINT
#include <cstdlib>  // NOLINT
#include <vector>  // NOLINT
#include "Assembler.hpp"
#include "Consumer.hpp"
#include "SharedNode.hpp"
#include "PrivateNode.hpp"

class Despachador : public Consumer<SharedNode*> {
 private:
  /* data */
 public:
  /**
   * @brief Constructor del despachador. Se encarga de ensamblar
   *        el mensaje y reproducirselo al cliente en pantalla.
  */
  Despachador(/* args */);

  /**
   * @brief Metodo sobreescrito de la clase Consumer. Permite
   *        consumir objetos que se encuentran en su cola.
   * @param shared_node
   * @return void
  */
  void consume(SharedNode* shared_node) override;

  /**
   * @brief Metodo que envia la solicitud al cliente.
   * @param shared_node
   * @return bool
  */  
  bool sendResponse(SharedNode* shared_node);

  /**
   * @brief Metodo sobreescrito de la clase Thread. Pone a correr
   *        el hilo.
   * @return void
  */  
  int run() override;

  ~Despachador();
};

#endif  // DESPACHADOR_HPP
