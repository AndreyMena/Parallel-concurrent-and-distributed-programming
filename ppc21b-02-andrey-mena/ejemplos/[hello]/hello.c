/*
 * Copyright 2021 Jeisson Hidalgo-Cespedes - Universidad de Costa Rica
 * Creates a secondary thread that greets in the standard output
 * 
 * Nota: Codigo hecho por el profesor Jeisson, enviado al repositorio
 * con el fin de ir completando los ejemplos e ir familiarizando con los
 * comits y comandos en terminal.
 */

#include <pthread.h>
#include <stdio.h>

void* run(void* data) {
  pthread_mutex_t* stdout_mutex = (pthread_mutex_t*)data;
  pthread_mutex_lock(stdout_mutex);
  printf("Hello from secondary thread, mutex at %p\n", data);
  pthread_mutex_unlock(stdout_mutex);

  return NULL;
}

int main(void) {
  pthread_t thread;
  pthread_mutex_t stdout_mutex;
  int error = pthread_mutex_init(&stdout_mutex, /*attr*/ NULL);
  if (error == 0) {
    error = pthread_create(&thread, NULL, run, &stdout_mutex);
    if (error == 0) {
      pthread_mutex_lock(&stdout_mutex);
      printf("Hello from main thread\n");
      pthread_mutex_unlock(&stdout_mutex);
      pthread_join(thread, NULL);
    } else {
      fprintf(stderr, "error: could not create a secondary thread\n");
    }
    pthread_mutex_destroy(&stdout_mutex);
  } else {
    fprintf(stderr, "error: could not init mutex\n");
  }
  return 0;
}

// lock == mutex: mutual exclusion
