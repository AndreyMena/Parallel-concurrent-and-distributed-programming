// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#include <stdbool.h>
#include <stdio.h>

#include "common.h"
#include "consumer.h"

void* consume(void* data) {
  simulation_t* simulation = (simulation_t*)data;
  size_t my_unit = 0;

  while (true) { 
    pthread_mutex_lock(&simulation->can_access_consumed_count);
    if (simulation->consumed_count < simulation->unit_count) {
      ++simulation->consumed_count;
    } else {
      pthread_mutex_unlock(&simulation->can_access_consumed_count);
      break;
    }
    pthread_mutex_unlock(&simulation->can_access_consumed_count);

    sem_wait(&simulation->can_consume);
    queue_dequeue(&simulation->queue, &my_unit);
    printf("\tConsumed %zu\n", my_unit);
    usleep(1000 * random_between(simulation->consumer_min_delay
      , simulation->consumer_max_delay));
  }

  return NULL;
}
