// Copyright 2021 Christopher Obando, Andrey Mena, Andrés Fallas
#include "Simulation.hpp"
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std; // NOLINT

Simulation::Simulation(/* args */) {
}

Simulation::~Simulation() {
}

int Simulation::run(char plate[20], double pTime, double pDifusividad,
 double h, double epsilon, string route, int thread_count) {
  route.append(plate);
  orgn_mtx = read_bin_file(route);

  double ctime;
  double ctime1;
  double ctime2;
  double diff;
  ofstream output;
  timestamp();
  diff = epsilon;
  //strcpy(output_filename, "out.txt"); // NOLINT

  // Se itera hasta que la nueva solución que satisface la condición de epsilon
  vector<string> vectJob = load_job_file(plate);
  temp_mtx = orgn_mtx;

  for (size_t i = 0; i < orgn_mtx.size(); i++) {
    for (size_t j = 0; j < orgn_mtx[1].size(); j++) {
      cout<< orgn_mtx[i][j];
      cout<< "    ";
    }
    cout<< endl;
  }
  ctime1 = cpu_time();

  this->calculateHeatTransfer(epsilon, diff, pTime, pDifusividad, thread_count, h);
  ctime2 = cpu_time();
  ctime = ctime2 - ctime1;
  string stime = to_string(ctime);  // tiempo en string
  string stamp = timestamp();  // fecha en string
  //cout<< stime<<endl;
  // función para escribir la solución en el archivo de salida.
  this->printOutput(plate, stime, stamp, vectJob);
  return 1;
}

void Simulation::calculateHeatTransfer(double epsilon, double diff,
  double pTime, double pDifusividad, int thread_count, double h) {
  int iterations = 0;
  int iterations_print = 1;  
  while ( epsilon <= diff ) {
    // se copian los datos a la matriz temporal
    std::copy(std::begin(orgn_mtx), std::end(orgn_mtx), std::begin(temp_mtx));

    // Proceso para el cálculo de la iteración

    diff = 0.0;  // inicializo la diferencia

    // Inicio de la seccion critica
    #pragma omp parallel for num_threads(thread_count) schedule(guided)
    for (size_t i = 1; i < orgn_mtx.size() - 1; i++) {
      for (size_t j = 1; j < orgn_mtx[1].size() - 1; j++) {
        orgn_mtx[i][j] = temp_mtx[i][j] + (pTime * pDifusividad) / pow(h, 2.0)*
        (temp_mtx[i-1][j]+temp_mtx[i][j+1]+temp_mtx[i+1][j]+temp_mtx[i][j-1]-4*temp_mtx[i][j]);

         // si la diferencia es menor al valor absoluto
        if (diff < fabs(orgn_mtx[i][j] - temp_mtx[i][j])) {
          diff = fabs(orgn_mtx[i][j] - temp_mtx[i][j]);
        }
      }
    }
    iterations++;
    if ( iterations == iterations_print ) {
      // cout << "  " << setw(8) << iterations
      //     << "  " << diff << "\n";
      iterations_print = 2 * iterations_print;
    }
  }  
}

void Simulation::printOutput(char output_filename[80], string stime, string stamp, vector<string> vectJob) {
  ofstream output;
  output.open(output_filename);
  for (size_t i = 0; i < vectJob.size(); i++) {
    output << vectJob[i];
  }
  output << stamp << endl;
  output << stime << endl;

  output << "\n";

  for (size_t i = 0; i < orgn_mtx.size(); i++) {
    for (size_t j = 0; j < orgn_mtx[1].size(); j++) {
      output << "  " << orgn_mtx[i][j];
    }
    output << "\n";
  }
  output.close();
}


/**
 * @brief Método para leer los valores de la matriz inicial
 * 
 * @param fileName 
 * @return void
 */
// deberá devolver la matriz cargada
vector<vector<double>> Simulation::read_bin_file(/*char plate[]*/
  string route ) {
  FILE * fichero;
  fichero = fopen(route.c_str(), "rb");
  vector<unsigned int> numeros;
  numeros.resize(2);
  vector<double>temperaturas;
  size_t resultado;
  int contador = 1;
  for (int i = 0; i < 2; ++i) {
    resultado = fread(&numeros[i], sizeof(unsigned int*), 1, fichero);
    if (resultado != 1) {
      break;
    }
  }
  temperaturas.resize(numeros[0]*numeros[1]);

  for (int i = 0; !feof(fichero); ++i) {
    resultado = fread(&temperaturas[i], sizeof(double*), 1, fichero);
    if (resultado != 1) {
      break;
    }
  }
  if (fclose(fichero) != 0) {
    cout << "No se ha podido cerrar el archivo" << endl;
  }
  cout << endl;
  int fila = numeros[0];
  int columna = numeros[1];
  contador = 0;
  vector<vector<double>> matriz(fila, vector<double>(columna));
  for (int i = 0; i < fila; i++) {
    for (int j = 0; j < columna; j++) {
      matriz[i][j] = temperaturas[contador];
      contador++;
    }
  }
  return matriz;
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

    for (size_t i = 0; i < vectorOfStrings.size(); i++) {
          // cout << vectorOfStrings[i] << endl;
    }
    string jobFile = vectorOfStrings[0];
  }
  return vectorOfStrings;
}

/**
 * @brief Tiempo de lectura de CPU
 * 
 * @return double 
 */
double Simulation::cpu_time() {
  double value;
  value = static_cast<double>(clock()) / static_cast<double>(CLOCKS_PER_SEC);
  return value;
}

/**
 * @brief Impresión de fecha y hora en la que se corre el código
 */
string Simulation::timestamp() {
# define TIME_SIZE 40
  std::string stampString;
  char time_buffer[TIME_SIZE];
  const struct std::tm *tm_ptr;
  std::time_t now;

  now = std::time(NULL);
  tm_ptr = std::localtime(&now);

  std::strftime(time_buffer, TIME_SIZE, "%Y/%m/%d", tm_ptr);
  // std::cout << time_buffer << "\n";
  int buffer_size = sizeof(time_buffer) / sizeof(char);
  stampString = convertToString(time_buffer, buffer_size);
  return stampString;
# undef TIME_SIZE
}

/**
 * @brief Metodo para convertir en string el 
 * buffer del time stamp generado
**/
string Simulation::convertToString(char* a, int size) {
  int i;
  string s = "";
  for (i = 0; i < size; i++) {
      s = s + a[i];
  }
  return s;
}
