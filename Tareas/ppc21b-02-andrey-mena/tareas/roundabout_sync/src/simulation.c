/** 
 * @copyright [2021] <Andrey Esteban Mena Espinoza>
*/
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <assert.h>
#include <semaphore.h>

#include "simulation.h"

typedef struct privateData {
  int64_t car_number;
  int64_t min_duration;
  int64_t max_duration;
  char enter_from;
  char exit_to;
  shared_data_t* shared_data;
}privateData_t;

int simulation_run(char (*strings)[20], int64_t cars, int64_t min, int64_t max,
  int64_t verbose, int64_t segmentCapacity, shared_data_t* sharedData) {
  int error = EXIT_SUCCESS;
  int64_t roundaboutCapacity = segmentCapacity*4;
  sharedData->cars_count = cars;
  sharedData->segment_capacity = segmentCapacity;
  sharedData->verboseBool = verbose;
  sharedData->directionSems = (sem_t*) calloc(4, sizeof(sem_t));
  error = sem_init(&sharedData->roundabout, 1, roundaboutCapacity-1);
  for (int64_t numOfSem = 0; numOfSem < 4; numOfSem++) {
    error = sem_init(&sharedData->directionSems[numOfSem], 1, segmentCapacity);
  }

  pthread_t* threads = (pthread_t*)
    malloc(sharedData->cars_count * sizeof(pthread_t));
  privateData_t* privateData = (privateData_t*)
    calloc(sharedData->cars_count, sizeof(privateData_t));
  if (threads && privateData && sharedData) {
    for (int64_t threadNumber = 0; threadNumber < cars; threadNumber++) {
      privateData[threadNumber].enter_from = strings[threadNumber][0];
      privateData[threadNumber].exit_to = strings[threadNumber][1];
      privateData[threadNumber].min_duration = min;
      privateData[threadNumber].max_duration = max;
      privateData[threadNumber].car_number = threadNumber;
      privateData[threadNumber].shared_data = sharedData;

      error = pthread_create(&threads[threadNumber], NULL
        , simulation_vehicle, &privateData[threadNumber]);
    }
  } else {
    printf("Error: Couldnt allocatote carData, threads or sharedData");
    error = 10;
  }
  destroyAll(sharedData, privateData, threads, cars);
  return error;
}

void* simulation_vehicle(void* data) {
  privateData_t* privateData = (privateData_t*) data;
  shared_data_t* shared_data = privateData->shared_data;

  sem_wait(&shared_data->roundabout);

  simulation_drive(privateData, shared_data);

  sem_post(&shared_data->roundabout);

  return NULL;
}

int simulation_drive(privateData_t* privateData, shared_data_t* shared_data) {
  int error = 0;
  char N = 'N'; char S = 'S'; char E = 'E'; char O = 'O';
  char number[200] = "";
  sprintf(number, "%"PRId64, privateData->car_number); //NOLINT
  strcat(shared_data->salida[privateData->car_number], number); //NOLINT
  strcat(shared_data->salida[privateData->car_number], " "); //NOLINT

  char str[10000] = "";
  str[0] = privateData->enter_from;
  str[1] = privateData->exit_to;

  // ENTRADA POR N

  strcat(shared_data->salida[privateData->car_number], str); //NOLINT
  strcat(shared_data->salida[privateData->car_number], ": "); //NOLINT
  srand(time(0));
  if (privateData->enter_from == N) {
    // Espera segmento N hasta semaforo en verde.
    sem_wait(&shared_data->directionSems[0]);
    // Arranca el vehiculo
    simulation_move(privateData->min_duration, privateData->max_duration);
    if (shared_data->verboseBool == 1) {
      printf("%"PRId64": %c\n", privateData->car_number, 'N');  // VERBOSE
    }
    strcat(shared_data->salida[privateData->car_number], "N"); //NOLINT
    char dir = 'x';
    int numDir = 0;
    sem_post(&shared_data->directionSems[0]);
    // Termina segmento, actualiza semaforo y se prepara por siguiente segmento
    while (dir != privateData->exit_to) {
      if (numDir == 0) {
        sem_wait(&shared_data->directionSems[1]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'O';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " O");  //NOLINT
        sem_post(&shared_data->directionSems[1]);
      }
      if (numDir == 1) {
        sem_wait(&shared_data->directionSems[2]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'S';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " S");  //NOLINT
        sem_post(&shared_data->directionSems[2]);
      }
      if (numDir == 2) {
        sem_wait(&shared_data->directionSems[3]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'E';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " E");  //NOLINT
        sem_post(&shared_data->directionSems[3]);
      }
      if (numDir == 3) {
        sem_wait(&shared_data->directionSems[0]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'N';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " N");  //NOLINT
        sem_post(&shared_data->directionSems[0]);
      }
      numDir++;
    }
  }

  // ENTRADA POR O

  if (privateData->enter_from == O) {
    // Espera segmento O hasta semaforo en verde.
    sem_wait(&shared_data->directionSems[1]);
    // Arranca el vehiculo
    simulation_move(privateData->min_duration, privateData->max_duration);
    if (shared_data->verboseBool == 1) {
      printf("%"PRId64": %c\n", privateData->car_number, 'O');  // VERBOSE
    }
    strcat(shared_data->salida[privateData->car_number], "O");  //NOLINT
    char dir = 'x';
    int numDir = 0;
    sem_post(&shared_data->directionSems[1]);
    // Termina segmento, actualiza semaforo y se prepara por siguiente segmento
    while (dir != privateData->exit_to) {
      if (numDir == 0) {
        sem_wait(&shared_data->directionSems[2]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'S';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " S");  //NOLINT
        sem_post(&shared_data->directionSems[2]);
      }
      if (numDir == 1) {
        sem_wait(&shared_data->directionSems[3]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'E';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " E");  //NOLINT
        sem_post(&shared_data->directionSems[3]);
      }
      if (numDir == 2) {
        sem_wait(&shared_data->directionSems[0]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'N';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " N");  //NOLINT
        sem_post(&shared_data->directionSems[0]);
      }
      if (numDir == 3) {
        sem_wait(&shared_data->directionSems[1]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'O';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " O");  //NOLINT
        sem_post(&shared_data->directionSems[1]);
      }
      numDir++;
    }
  }

  // ENTRADA POR S

  if (privateData->enter_from == S) {
    // Espera segmento S hasta semaforo en verde.
    sem_wait(&shared_data->directionSems[2]);
    // Arranca el vehiculo
    simulation_move(privateData->min_duration, privateData->max_duration);
    if (shared_data->verboseBool == 1) {
      printf("%"PRId64": %c\n", privateData->car_number, 'S');  // VERBOSE
    }
    strcat(shared_data->salida[privateData->car_number], "S"); //NOLINT
    char dir = 'x';
    int numDir = 0;
    sem_post(&shared_data->directionSems[2]);
    // Termina segmento, actualiza semaforo y se prepara por siguiente segmento
    while (dir != privateData->exit_to) {
      if (numDir == 0) {
        sem_wait(&shared_data->directionSems[3]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'E';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " E");  //NOLINT
        sem_post(&shared_data->directionSems[3]);
      }
      if (numDir == 1) {
        sem_wait(&shared_data->directionSems[0]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'N';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " N");  //NOLINT
        sem_post(&shared_data->directionSems[0]);
      }
      if (numDir == 2) {
        sem_wait(&shared_data->directionSems[1]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'O';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " O");  //NOLINT
        sem_post(&shared_data->directionSems[1]);
      }
      if (numDir == 3) {
        sem_wait(&shared_data->directionSems[2]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'S';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " S");  //NOLINT
        sem_post(&shared_data->directionSems[2]);
      }
      numDir++;
    }
  }

  // ENTRADA POR E

  if (privateData->enter_from == E) {
    // Espera segmento E hasta semaforo en verde.
    sem_wait(&shared_data->directionSems[3]);
    // Arranca el vehiculo
    simulation_move(privateData->min_duration, privateData->max_duration);
    if (shared_data->verboseBool == 1) {
      printf("%"PRId64": %c\n", privateData->car_number, 'E');  // VERBOSE
    }
    strcat(shared_data->salida[privateData->car_number], "E"); //NOLINT
    char dir = 'x';
    int numDir = 0;
    sem_post(&shared_data->directionSems[3]);
    // Termina segmento, actualiza semaforo y se prepara por siguiente segmento
    while (dir != privateData->exit_to) {
      if (numDir == 0) {
        sem_wait(&shared_data->directionSems[0]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'N';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " N");  //NOLINT
        sem_post(&shared_data->directionSems[0]);
      }
      if (numDir == 1) {
        sem_wait(&shared_data->directionSems[1]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'O';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " O");  //NOLINT
        sem_post(&shared_data->directionSems[1]);
      }
      if (numDir == 2) {
        sem_wait(&shared_data->directionSems[2]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'S';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " S");  //NOLINT  //NOLINT
        sem_post(&shared_data->directionSems[2]);
      }
      if (numDir == 3) {
        sem_wait(&shared_data->directionSems[3]);
        // Arranca el vehiculo
        simulation_move(privateData->min_duration, privateData->max_duration);
        dir = 'E';
        if (shared_data->verboseBool == 1) {
          printf("%"PRId64": %c\n", privateData->car_number, dir);  // VERBOSE
        }
        strcat(shared_data->salida[privateData->car_number], " E");  //NOLINT
        sem_post(&shared_data->directionSems[3]);
      }
      numDir++;
    }
  }
  return error;
}

void toStringFinal(int64_t cars, shared_data_t* sharedData) {
  for (int64_t index = 0; index < cars; index++) {
    printf("%s\n", sharedData->salida[index]);
  }
}

void simulation_move(int64_t min, int64_t max) {
  //https://www.includehelp.com/c-programs/generate-random-numbers-within-a-range.aspx  //NOLINT
  int num = (rand() % (min -  //NOLINT
    max)) + min;
  // printf("%d\n", num);
  usleep(num*1000);
}

simulation_t* simulation_create(void) {
  simulation_t* simulation = (simulation_t*)
    calloc(1, sizeof(simulation_t));
  return simulation;
}

void simulation_destroy(simulation_t* simulation) {
  assert(simulation);
  free(simulation);
}

void destroyAll(shared_data_t* sharedData, privateData_t* privateData,
  pthread_t* threads, int64_t cars) {
  for (int64_t threadNumber = 0; threadNumber < cars; threadNumber++) {
    pthread_join(threads[threadNumber], NULL);
  }
  for (int64_t index = 0; index < 4; index++) {
    sem_destroy(&sharedData->directionSems[index]);
  }
  free(sharedData->directionSems);
  toStringFinal(cars, sharedData);
  sem_destroy(&sharedData->roundabout);
  free(threads);
  free(sharedData);
  free(privateData);
}
