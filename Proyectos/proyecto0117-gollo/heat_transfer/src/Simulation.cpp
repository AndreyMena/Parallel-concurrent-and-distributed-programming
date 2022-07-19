/*
 * Copyright 2022 Gollo
 */
#include "Simulation.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <mpi.h> // NOLINT

using namespace std;  // NOLINT

Simulation::Simulation(/* args */) {}

Simulation::~Simulation() {}

int Simulation::sendDynamic(int& process_number, int& process_count,
  int& thread_count, string& route, ofstream& outputJobTsv,
  ifstream& jobs, vector<string>& vectPlates, vector<double>& vectPtimes,
  vector<double>& vectPdifusividad, vector<double>& vecth,
  vector<double>& vecteEpsilon, int& lineas, vector<int>& orderIterations) {
  process_number = process_number;
  int error = EXIT_SUCCESS;
  int index = 0;
  while (index < lineas) {
    MPI_Status status;
    int couldContinue = -1;
    if (MPI_Recv(&couldContinue, 1, MPI_INT, MPI_ANY_SOURCE
      , /*tag*/ 0, MPI_COMM_WORLD, &status) != MPI_SUCCESS ) {
      cerr <<  "could not receive message";
    }
    int readyForMore = 1;
    if (MPI_Send(&readyForMore, 1, MPI_INT, /*target*/ status.MPI_SOURCE
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      cerr << "could not send message";
    }
    // Ahora se envian trabajos
    // RUTA
    int lenght = route.length();
    if (MPI_Send(&lenght, 1, MPI_INT, /*target*/ status.MPI_SOURCE
      , /*tag*/ 1, MPI_COMM_WORLD) != MPI_SUCCESS) {
      cerr << "could not send message";
    }
    if (MPI_Send(route.data(), route.length(), MPI_CHAR,
      /*target*/status.MPI_SOURCE
      , /*tag*/ 2, MPI_COMM_WORLD) != MPI_SUCCESS) {
      cerr << "could not send message";
    }
    // plate
    lenght = vectPlates[index].length();  // RUTA
    if (MPI_Send(&lenght, 1, MPI_INT, /*target*/ status.MPI_SOURCE
      , /*tag*/ 3, MPI_COMM_WORLD) != MPI_SUCCESS) {
      cerr << "could not send message";
    }
    if (MPI_Send(vectPlates[index].data(), vectPlates[index].length(),
      MPI_CHAR, /*target*/ status.MPI_SOURCE
      , /*tag*/ 4, MPI_COMM_WORLD) != MPI_SUCCESS) {
      cerr << "could not send message";
    }
    // Ptime
    if (MPI_Send(&vectPtimes[index], 1, MPI_DOUBLE,
      /*target*/ status.MPI_SOURCE
      , /*tag*/ 5, MPI_COMM_WORLD) != MPI_SUCCESS) {
      cerr << "could not send message";
    }
    // Pdifusividad
    if (MPI_Send(&vectPdifusividad[index], 1, MPI_DOUBLE,
      /*target*/ status.MPI_SOURCE
      , /*tag*/ 6, MPI_COMM_WORLD) != MPI_SUCCESS) {
      cerr << "could not send message";
    }
    // h
    if (MPI_Send(&vecth[index], 1, MPI_DOUBLE, /*target*/ status.MPI_SOURCE
      , /*tag*/ 7, MPI_COMM_WORLD) != MPI_SUCCESS) {
      cerr << "could not send message";
    }
    // epsilon
    if (MPI_Send(&vecteEpsilon[index], 1, MPI_DOUBLE,
      /*target*/ status.MPI_SOURCE
      , /*tag*/ 8, MPI_COMM_WORLD) != MPI_SUCCESS) {
      cerr << "could not send message";
    }
    // thread_count
    if (MPI_Send(&thread_count, 1, MPI_INT, /*target*/ status.MPI_SOURCE
      , /*tag*/ 9, MPI_COMM_WORLD) != MPI_SUCCESS) {
      cerr << "could not send message";
    }
    orderIterations.push_back(status.MPI_SOURCE);
    index++;
  }

  // Condiciones de parada.
  for (int process = 0; process < process_count; process++) {
    int condParada = 0;
    int couldContinue = -1;
    if (MPI_Recv(&couldContinue, 1, MPI_INT, MPI_ANY_SOURCE
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      cerr <<  "could not receive message";
    }
    if (MPI_Send(&condParada, 1, MPI_INT, /*target*/ process
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      cerr << "could not send message";
    }
  }
  return error;
}

int Simulation::run(char plate[20], double pTime, double pDifusividad,
  double h, double epsilon, string route, int thread_count
  , int process_number) {
  process_number = process_number;
  route.append(plate);
  uint64_t fila = -1;
  uint64_t columna = -1;
  orgn_mtx = read_bin_file(route, fila, columna);
  int iterations = 0;  // k
  double diff;
  diff = epsilon;
  // Se itera hasta que la nueva solución que satisface la condición de epsilon
  vector<string> vectJob = load_job_file(plate);

  temp_mtx = orgn_mtx;

  this->calculateHeatTransfer(epsilon, diff, pTime, pDifusividad, thread_count,
                              h, iterations);

  // cout << "Soy proceso: " << process_number<< " y tengo: "<< route <<" "
  // << plate << " "<< pTime<<" "<<pDifusividad << " "<< h <<" "
  // << epsilon << endl;

  // función para escribir la solución en el archivo de salida.
  this->printOutput(plate, fila, columna, iterations);

  if (MPI_Send(&iterations, 1, MPI_INT, /*target*/ 0
    , /*tag*/ 10, MPI_COMM_WORLD) != MPI_SUCCESS) {
    cerr << "could not send message";
  }

  double secondsTime = iterations * pTime;
  stringstream resultTime = calculateTime(secondsTime);

  return 1;
}

stringstream Simulation::calculateTime(double secondsTime) {
  double timeAux = secondsTime; double years = 0; double months = 0;
  double days = 0; double hours = 0; double minutes = 0; double seconds = 0;
  if (timeAux >= 525600 * 60) {
    while (timeAux >= 525600 * 60) {
      timeAux = timeAux - 525600 * 60;
      years++;
    }
  }
  if (timeAux >= 43200 * 60) {
    while (timeAux >= 43200 * 60) {
      timeAux = timeAux - 43200 * 60;
      months++;
    }
  }
  if (timeAux >= 1440 * 60) {
    while (timeAux >= 1440 * 60) {
      timeAux = timeAux - 1440 * 60;
      days++;
    }
  }
  if (timeAux >= 60 * 60) {
    while (timeAux >= 60 * 60) {
      timeAux = timeAux - 60 * 60;
      hours++;
    }
  }
  if (timeAux >= 1 * 60) {
    while (timeAux >= 1 * 60) {
      timeAux = timeAux - 1 * 60;
      minutes++;
    }
  }
  if (timeAux >= 1) {
    while (timeAux >= 1) {
      timeAux = timeAux - 1;
      seconds++;
    }
  }
  std::stringstream resultTime;
  resultTime << setfill('0'); resultTime << setw(4);
  resultTime << years << "/";  // Years
  resultTime << setw(2); resultTime << months << "/";  // Months
  resultTime << setw(2); resultTime << days << "\t";  // Days
  resultTime << setw(2); resultTime << hours << ":";  // Hours
  resultTime << setw(2); resultTime << minutes << ":";  // Minutes
  resultTime << setw(2); resultTime << seconds << endl;  // Seconds
  return resultTime;
}

void Simulation::calculateHeatTransfer(double epsilon, double diff,
                                       double pTime, double pDifusividad,
                                       int thread_count, double h,
                                       int& iterations) {
  thread_count = thread_count;
  int iterations_print = 1;
  while (epsilon <= diff) {
    // Se copian los datos a la matriz temporal
    std::copy(std::begin(orgn_mtx), std::end(orgn_mtx), std::begin(temp_mtx));

    // Proceso para el cálculo de la iteración
    diff = 0.0;  // inicializo la diferencia

  // Inicio de la seccion critica
  // #pragma omp parallel for num_threads(thread_count)
    for (size_t i = 1; i < orgn_mtx.size() - 1; i++) {
      for (size_t j = 1; j < orgn_mtx[1].size() - 1; j++) {
        // #pragma critical(canCalculates)
        // {
          orgn_mtx[i][j] =
              temp_mtx[i][j] +
              (pTime * pDifusividad) / pow(h, 2.0) *
                  (temp_mtx[i - 1][j] + temp_mtx[i][j + 1] +
                  temp_mtx[i + 1][j] +
                  temp_mtx[i][j - 1] - 4 * temp_mtx[i][j]);

          // si la diferencia es menor al valor absoluto
          if (diff < fabs(orgn_mtx[i][j] - temp_mtx[i][j])) {
            diff = fabs(orgn_mtx[i][j] - temp_mtx[i][j]);
          }
        // }
      }
    }
    iterations++;
    if (iterations == iterations_print) {
      iterations_print = 2 * iterations_print;
    }
  }
}

void Simulation::printOutput(char output_filename[80], uint64_t fila,
                             uint64_t columna, int iterations) {
  ofstream output;
  std::string plateName(output_filename);
  plateName = plateName.erase(plateName.find("."), 4);
  plateName += "-";
  plateName += to_string(iterations);
  plateName += ".bin";

  ofstream newPlate(plateName, fstream::out | fstream::binary);
  if (newPlate.is_open()) {
    newPlate.write(reinterpret_cast<char*>(&fila), sizeof(fila));
    // Se lee columnas
    newPlate.write(reinterpret_cast<char*>(&columna), sizeof(columna));
    for (uint64_t filas = 0; filas < fila; ++filas) {
      for (uint64_t columnas = 0; columnas < columna; ++columnas) {
        newPlate.write(reinterpret_cast<char*>(&orgn_mtx[filas][columnas]),
                       sizeof(orgn_mtx[filas][columnas]));
      }
      newPlate.close();
    }
  } else {
    cout << "No se pudo abrir el archivo binario" << endl;
  }
}

vector<vector<double>> Simulation::read_bin_file(string route, uint64_t& fila,
                                                 uint64_t& columna) {
  std::ifstream jobFile(route, std::fstream::in | std::fstream::binary);
  uint64_t filaPlaca = 1, columnaPlaca = 1;
  vector<vector<double>> matrizMala(filaPlaca, vector<double>(columnaPlaca));
  if (jobFile.is_open()) {
    // Se leera las fila
    jobFile.read(reinterpret_cast<char*>(&filaPlaca), sizeof(filaPlaca));
    // Se leera las columna
    jobFile.read(reinterpret_cast<char*>(&columnaPlaca), sizeof(columnaPlaca));
    fila = filaPlaca;
    columna = columnaPlaca;
    double valorCeldaPlaca;
    vector<vector<double>> matriz(filaPlaca, vector<double>(columnaPlaca));
    for (uint64_t filas = 0; filas < fila; ++filas) {
      for (uint64_t columnas = 0; columnas < columna; ++columnas) {
        jobFile.read(reinterpret_cast<char*>(&valorCeldaPlaca), sizeof(double));
        matriz[filas][columnas] = valorCeldaPlaca;
      }
    }
    return matriz;
  } else {
    cout << "No se pudo abrir el archivo binario" << endl;
  }
  return matrizMala;
}

/**
 * @brief Metodo que se encarga de leer
 * el archivo de trabajo
 */
vector<string> Simulation::load_job_file(char plate[20]) {
  std::ifstream jobFile(plate, ios::in);
  vector<string> vectorOfStrings;
  if (jobFile.is_open()) {
    string temp;

    while (getline(jobFile, temp)) {
      int i = 0;
      vectorOfStrings.push_back(temp);
      i++;
    }
    jobFile.close();

    string jobFile = vectorOfStrings[0];
  }
  return vectorOfStrings;
}

int Simulation::receiveDynamic(int process_number, int process_count) {
  process_count = process_count;
  int error = EXIT_SUCCESS;
  while (true) {
    int couldContinue = 1;
    if (MPI_Send(&couldContinue, 1, MPI_INT, /*target*/ 0, /*tag*/ 0,
                 MPI_COMM_WORLD) != MPI_SUCCESS) {
      cerr << "could not send message";
    }
    int readyForMore = -1;
    if (MPI_Recv(&readyForMore, 1, MPI_INT, MPI_ANY_SOURCE, /*tag*/ 0,
                 MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      cerr << "could not receive message";
    }
    if (readyForMore == 0) {
      break;
    }
    // Recepcion de trabajo
    // routeChar
    int lenght = -1;
    if (MPI_Recv(&lenght, 1, MPI_INT, 0, /*tag*/ 1, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      cerr << "could not receive message";
    }
    char* routeChar = (char*) calloc(lenght, sizeof(char));  // NOLINT
    if (MPI_Recv(routeChar, lenght + 1, MPI_CHAR, 0, /*tag*/ 2, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      cerr << "could not receive message";
    }
    string route(routeChar);
    // Plate
    lenght = -1;
    if (MPI_Recv(&lenght, 1, MPI_INT, 0, /*tag*/ 3, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      cerr << "could not receive message";
    }
    char* plate = (char*)calloc(lenght, sizeof(char));  // NOLINT
    if (MPI_Recv(plate, lenght, MPI_CHAR, 0, /*tag*/ 4, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      cerr << "could not receive message";
    }
    // Ptimes
    double pTime = -1;
    if (MPI_Recv(&pTime, 1, MPI_DOUBLE, 0, /*tag*/ 5, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      cerr << "could not receive message";
    }
    // Pdifusivad
    double pDifusividad = -1;
    if (MPI_Recv(&pDifusividad, 1, MPI_DOUBLE, 0, /*tag*/ 6, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      cerr << "could not receive message";
    }
    // h
    double h = -1;
    if (MPI_Recv(&h, 1, MPI_DOUBLE, 0, /*tag*/ 7, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      cerr << "could not receive message";
    }
    // epsilon
    double epsilon = -1;
    if (MPI_Recv(&epsilon, 1, MPI_DOUBLE, 0, /*tag*/ 8, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      cerr << "could not receive message";
    }
    // thread_count
    int thread_count = -1;
    if (MPI_Recv(&thread_count, 1, MPI_INT, 0, /*tag*/ 9, MPI_COMM_WORLD,
                 MPI_STATUS_IGNORE) != MPI_SUCCESS) {
      cerr << "could not receive message";
    }

    this->run(plate, pTime, pDifusividad, h, epsilon, route, thread_count,
              process_number);
    free(plate);
  }
  return error;
}
