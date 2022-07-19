// Copyright 2021 Jeisson Hidalgo-Cespedes. Universidad de Costa Rica. CC BY 4.0

#include "Empaquetador.hpp"
#include "HttpResponse.hpp"
#include <string>  // NOLINT

#include "SharedNode.hpp"
#include "PrivateNode.hpp"

Empaquetador::Empaquetador(/* args */) {}

Empaquetador::~Empaquetador() {}

void Empaquetador::consume(PrivateNode* private_node) {
  private_nodeLocal = private_node;
  pthread_mutex_lock(&private_nodeLocal->shared_node->can_acces_totalNumbers);
  // Aumenta la variable para comparar si ha procesado todos los numeros.
  private_nodeLocal->shared_node->procces_numbers++;
  if (private_nodeLocal->shared_node->procces_numbers ==
    private_nodeLocal->shared_node->total_numbers) {
    // Una vez verifica que ha procesado todos los numeros, produce.
    produce(private_nodeLocal->shared_node);
  }
  pthread_mutex_unlock(&private_nodeLocal->shared_node->can_acces_totalNumbers);
  delete private_node;
}

int Empaquetador::run() {
  this->consumeForever();
  return 0;
}
