/*
* Copyright 2022 Gollo
*/
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <string>
#include <omp.h>  // NOLINT
#include <mpi.h>  // NOLINT
#include <sstream>
#include <time.h>  // NOLINT

#include <vector>
#include <unistd.h>  // NOLINT


#include "Simulation.hpp"

using namespace std;  // NOLINT

int printJob(int& process_number, int& process_count, ofstream& outputJobTsv,
             vector<string>& vectPlates, vector<double>& vectPtimes,
             vector<double>& vectPdifusividad, vector<double>& vecth,
             vector<double>& vecteEpsilon, int& lineas,
             vector<int>& orderIterations, Simulation& simulation) {
  process_number = process_number;
  process_count = process_count;
  int error = EXIT_SUCCESS;
  for (int index = 0; index < lineas; index++) {
    outputJobTsv << vectPlates[index] << "\t" << vectPtimes[index] << "\t"
                 << vectPdifusividad[index] << "\t" << vecth[index] << "\t"
                 << vecteEpsilon[index] << "\t";

    int iterations = -1;
    if (MPI_Recv(&iterations, 1, MPI_INT, orderIterations[index], /*tag*/ 10,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      cerr << "could not receive message";
    }

    outputJobTsv << iterations << "\t";

    double secondsTime = iterations * vectPtimes[index];
    stringstream resultTime = simulation.calculateTime(secondsTime);

    outputJobTsv << resultTime.str();
  }
  return error;
}

string readRoute(string& route) {
  char ruta2[15];
  int contador = 0;
  size_t i = 0, encontrado = 0;
  string jobtxt;
  while (i < route.length() && !encontrado) {
    ruta2[i] = route.at(i);
    if (route.at(i) == '/') {
      ++contador;
    }
    if (contador == 2) {
      encontrado = 1;
      jobtxt = route.substr(i + 1, route.length() - i);
    }
    ++i;
  }
  route = ruta2;
  jobtxt = jobtxt.substr(0, 7);
  jobtxt.append("tsv");
  return jobtxt;
}

int readJob(Simulation& simulation, int& process_number, int& process_count,
            int& thread_count, string& route, ofstream& outputJobTsv,
            ifstream& jobs, vector<string>& vectPlates,
            vector<double>& vectPtimes, vector<double>& vectPdifusividad,
            vector<double>& vecth, vector<double>& vecteEpsilon) {
  thread_count = thread_count;
  route = route;
  simulation = simulation;
  process_number = process_number;
  process_count = process_count;
  char plate[20];
  double pTime;
  double pDifusividad;
  double h;
  double epsilon;
  int lineas = 0;
  if (jobs.fail()) {
    EXIT_FAILURE;
  } else {
    while (!jobs.eof()) {
      jobs >> plate >> pTime >> pDifusividad >> h >> epsilon;
      if (pTime < 0) {
        break;
      }

      lineas++;

      vectPlates.push_back(plate);
      vectPtimes.push_back(pTime);
      vectPdifusividad.push_back(pDifusividad);
      vecth.push_back(h);
      vecteEpsilon.push_back(epsilon);
      pTime = -1;
    }
    jobs.close();
  }
  return lineas;
}

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    int process_number = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);
    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);
    int thread_count = omp_get_max_threads();
    if (argc >= 3) {
      thread_count = atoi(argv[2]);
    } else {
      if (argc < 2) {
        cerr << "Parametros insuficientes" << endl;
      }
    }

    Simulation simulation;
    int lineas = 0;
    if (process_number == 0) {
      string route(argv[1]);
      string jobtxt = readRoute(route);
      ofstream outputJobTsv(jobtxt);  // job###.tsv
      ifstream jobs;
      string buffer;
      jobs.open(argv[1], ios::in);

      vector<string> vectPlates;
      vector<double> vectPtimes;
      vector<double> vectPdifusividad;
      vector<double> vecth;
      vector<double> vecteEpsilon;
      vector<int> orderIterations;

  // Start time.
  struct timespec start, finish;
  clock_gettime(CLOCK_MONOTONIC, &start);

      lineas = readJob(simulation, process_number, process_count, thread_count,
                       route, outputJobTsv, jobs, vectPlates, vectPtimes,
                       vectPdifusividad, vecth, vecteEpsilon);

      simulation.sendDynamic(process_number, process_count, thread_count, route,
                             outputJobTsv, jobs, vectPlates, vectPtimes,
                             vectPdifusividad, vecth, vecteEpsilon, lineas,
                             orderIterations);

      printJob(process_number, process_count, outputJobTsv, vectPlates,
               vectPtimes, vectPdifusividad, vecth, vecteEpsilon, lineas,
               orderIterations, simulation);
  clock_gettime(CLOCK_MONOTONIC, &finish);
  double duration = finish.tv_sec - start.tv_sec +
    (finish.tv_nsec - start.tv_nsec) / 1e9;
  fprintf(stderr, "Execution time: %.9lf\n", duration);
  // Finish time.

    } else {
      simulation.receiveDynamic(process_number, process_count);
    }
    // cout <<"Termino : " << process_number <<endl;
    MPI_Finalize();
  } else {
    cerr << "error: could not init MPI";
    error = EXIT_FAILURE;
  }
  return error;
}

