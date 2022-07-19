// Copyright 2022 Christopher Obando Salgado,
//                Jose Alexis Valerio Ramirez,
//                Andrey Esteban Mena Espinoza,
//                Adrian Hutt Orozco.
//                Universidad de Costa Rica.

#ifndef PRIVATENODE_HPP
#define PRIVATENODE_HPP

#include <cstdint>

struct SharedNode;
/**
 * @brief The message to be transmitted by the simulated network
 */
struct PrivateNode {
 public:
  /**
   * @brief Puntero al nodo compartido donde se almacenas las respuestas.
  */
  SharedNode* shared_node;

  /**
   * @brief Numero especifico del hilo que sera procesado.
  */  
  int64_t number;

  /**
   * @brief Indice que corresponde a la posicion donde se ingresan los datos.
  */    
  int64_t index;

 public:
  /// Convenience constructor
  PrivateNode() {
  }

  /// @brief Operator==
  /// @param other PrivateNode&
  /// @return true if this message is equals to the given
  inline bool operator==(const PrivateNode& other) const {
    return this->index == other.index
      && this->number == other.number
      && this->shared_node == other.shared_node;
  }
};

#endif  // PRIVATENODE_HPP
