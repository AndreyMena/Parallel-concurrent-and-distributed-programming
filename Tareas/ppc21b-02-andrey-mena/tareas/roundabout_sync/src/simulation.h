#ifndef SIMULATION_H
#define SIMULATION_H
#include <stdint.h>
#include <semaphore.h>
#include "array_int.h"

/** 
 * @copyright [2021] <Andrey Esteban Mena Espinoza>
*/

/**
 * @brief Estructura de datos de memoria compartida.
*/
typedef struct shared_data {
  int64_t segment_capacity;
  uint64_t cars_count;  // cantidad de hilos pedidos.
  char salida[10000][1000];  // Entradas de hasta 1000 lineas y mil caracteres.
  sem_t roundabout;
  sem_t* directionSems;
  int64_t verboseBool;
}shared_data_t;

/**
 * @brief Estructura de datos obtener objeto simulation.
*/
typedef struct simulation {
}simulation_t;

/**
 * @brief Estructura de datos para cada hilo, vehiculo.
*/
typedef struct privateData privateData_t;

/**
 * @brief Subrutina que y redirige el programa.
 * @param strings, Areglo de string con las entradas.
 * @param cars Cantidad de vehiculos, threadCount.
 * @param min, minimo de tiempo de duracion por segmento.
 * @param max, maximo de tiempo de duracion por segmento.
 * @param verbose, Descripcion detallada, bool.
 * @param segmentCapacity, Capacidad del segmento
 * @param sharedData, Memoria compartida
 * @return void
*/
int simulation_run(char (*strings)[20], int64_t cars, int64_t min, int64_t max,
  int64_t verbose, int64_t segmentCapacity, shared_data_t* sharedData);

/**
 * @brief Subrutina que empieza el vehiculo
 * @param void* data, un lugar de la memoria
 * @return void*
*/
void* simulation_vehicle(void* data);

/**
 * @brief Subrutina que comienza a manejar el vehiculo
 * @param privateData memoria privada de cada hilo.
 * @param shared_data memoria compartida.
 * @return int
*/
int simulation_drive(privateData_t* privateData, shared_data_t* shared_data);

/**
 * @brief Subrutina que mueve el vehiculo de acuerdo a su velocidad
 * @param min minimo de tiempo de duracion por segmento.
 * @param max maximo de tiempo de duracion por segmento.
 * @return void
*/
void simulation_move(int64_t min, int64_t max);

/**
 * @brief Subrutina para destruir objetos creados.
 * @param privateData memoria privada de cada hilo.
 * @param shared_data memoria compartida.
 * @param threads hilos
 * @param cars cantidad de carros.
 * @return void
*/
void destroyAll(shared_data_t* sharedData, privateData_t* privateData,
  pthread_t* threads, int64_t cars);

/**
 * @brief Subrutina para crear objeto simulation
 * @return v*
*/
simulation_t* simulation_create(void);

/**
 * @brief Subrutina para destruir objeto simulation
 * @param simulation minimo de tiempo de duracion por segmento.
 * @return void
*/
void simulation_destroy(simulation_t* simulation);

#endif  // SIMULATION_H
