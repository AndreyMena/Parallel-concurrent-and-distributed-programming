// Copyright 2022 Christopher Obando Salgado,
//                Jose Alexis Valerio Ramirez,
//                Andrey Esteban Mena Espinoza,
//                Adrian Hutt Orozco.
//                Universidad de Costa Rica.

#ifndef SHAREDNODE_HPP
#define SHAREDNODE_HPP

#include "HttpResponse.hpp"
#include <cstdint>  // NOLINT
#include <vector>  // NOLINT
#include "GoldbachStruct.hpp"


/**
 * @brief Es el objeto productor del empaquetador, y el consumido
 *        por el despachador, en el se encuentra el httpResponse
 *        que es donde se ensambla el mensaje.
 */
struct SharedNode {
 public:
  /**
   * @brief Vector de numeros leido en el HttpRequest
  */
  std::vector<int64_t> numbers;

  /**
   * @brief Vector de que almacena las estructuras de golbach, donde
   *        se encuentras las sumas de cada numero.
  */  
  std::vector<GoldbachStruct> goldbachsStructures;

  /**
   * @brief Respuesta que sera enviada al usuario por medio de la pagina.
  */  
  HttpResponse& httpResponse;

  /**
   * @brief Mutex que utiliza el empaquetador para verificar que las variables
   *        ya han sido completamente procesadas.
  */   
  pthread_mutex_t can_acces_totalNumbers;

  /**
   * @brief Cantidad de numeros totales que contiene la respuesta.
  */     
  size_t total_numbers;

  /**
   * @brief Variable aumentada dentro del mutex para verificar cuantos
   *        numeros han sido procesados.
  */     
  size_t procces_numbers;

 public:
  /// Convenience constructor
  /// @param httpResponse HttpResponse&
  explicit SharedNode(HttpResponse& httpResponse)
    : httpResponse(httpResponse) {
  }

  /// @brief Operator==
  /// @param other SharedNode&
  /// @return true if this message is equals to the given
  inline bool operator==(const SharedNode& other) const {
    return this->numbers == other.numbers
      && this->goldbachsStructures == other.goldbachsStructures
      && this->httpResponse == other.httpResponse
      && this->procces_numbers == other.procces_numbers
      && this->total_numbers == other.total_numbers;
  }
};

#endif  // SHAREDNODE_HPP
