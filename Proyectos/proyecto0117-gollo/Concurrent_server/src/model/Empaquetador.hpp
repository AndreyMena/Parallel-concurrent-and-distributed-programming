// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#ifndef EMPAQUETADOR_HPP
#define EMPAQUETADOR_HPP
#include "HttpResponse.hpp"
#include <cstdint>  // NOLINT
#include <cstdlib>  // NOLINT
#include <vector>  // NOLINT
#include "Assembler.hpp"
#include "Consumer.hpp"
#include "SharedNode.hpp"
#include "PrivateNode.hpp"

class Empaquetador : public Assembler<PrivateNode*, SharedNode*> {
 private:
    /* data */

 public:
  /**
   * @brief Constructor del empaquetador, se encarga de ensamblar
   *        el mensaje de respuestas
  */
  Empaquetador(/* args */);

  /**
   * @brief Puntero al nodo actual del que se extraen los numeros.
  */
  PrivateNode* private_nodeLocal;

  /**
   * @brief Metodo sobreescrito de la clase Consumer. Permite
   *        consumir objetos que se encuentran en su cola.
   * @param private_node
   * @return void
  */
  void consume(PrivateNode* private_node) override;

  /**
   * @brief Metodo sobreescrito de la clase Thread. Pone a correr
   *        el hilo.
   * @return void
  */  
  int run() override;

  /**
   * @brief Destructor del empaquetador
  */
  ~Empaquetador();
};

#endif  //  EMPAQUETADOR_HPP
