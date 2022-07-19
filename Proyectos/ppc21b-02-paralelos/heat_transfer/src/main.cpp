// Copyright 2021 Christopher Obando, Andrey Mena, Andrés Fallas
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <string>
#include <omp.h> // NOLINT
#include <time.h> // NOLINT


#include <vector>
#include <unistd.h> // NOLINT

#include "Simulation.hpp"


using namespace std; // NOLINT

int main(int argc, char *argv[]);
double cpu_time();
string timestamp();
vector<string> load_job_file(string archivo);
vector<vector<double>> read_bin_file(int argc, char *argv[]);

int main(int argc, char *argv[]) {
  char plate[20];
  double pTime;
  double pDifusividad;
  double h;
  double epsilon;
  if (argc < 1) {
    cout << "Parametros insuficientes" << endl;
  }
  string route(argv[1]);
  char ruta2[15];
  int contador = 0;
  size_t i = 0, encontrado = 0;
  while (i < route.length() && !encontrado) {
      ruta2[i] = route.at(i);
      if (route.at(i) == '/') {
          ++contador;
      }
      if (contador == 2) {
          encontrado = 1;
      }
      ++i;
  }
  // cout<<ruta2<<endl;
  route = ruta2;

  ifstream jobs;
  // char buffer[200];  /// tama;o del plate
  string buffer;
  jobs.open(argv[1], ios::in);

  int lineas = 0;

  int thread_count = omp_get_max_threads();
  if (argc >= 3) {
    thread_count = atoi(argv[2]);
  }
  cout << thread_count;
  
  struct timespec star_time, finish_time;
  clock_gettime(CLOCK_MONOTONIC, & star_time);

  Simulation simulation;
  double ctime1 = simulation.cpu_time();
  string buff;
  if (jobs.fail()) {
    EXIT_FAILURE;
  } else {
    while (!jobs.eof()) {
      jobs >> plate >> pTime >> pDifusividad >> h >> epsilon;

      int error = simulation.run(plate, pTime, pDifusividad, h,
        epsilon, route, thread_count);
      if (error == 0) {
        cout<< "Error, no logro correr simulation run" << endl;
      }
      getline(jobs, buff);
      lineas++;
    }
    jobs.close();
  }
  clock_gettime(CLOCK_MONOTONIC, &finish_time);
  double duracion = finish_time.tv_sec - star_time.tv_sec +
    (finish_time.tv_nsec - star_time.tv_nsec) / 1e9;
  
  cout<< "Execution time: "<< duracion<< endl;


  cout << endl;
  return 0;
}
