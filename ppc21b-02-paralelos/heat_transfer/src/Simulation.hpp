// Copyright 2021 Christopher Obando Andrey Mena Andrés Fallas

#include <ctime>
#include <vector>
#include <string>
#include <cstring>

using namespace std; // NOLINT

class Simulation {
private:
    /* data */
  vector<vector<double>> orgn_mtx; // matriz original
  vector<vector<double>> temp_mtx; // matriz temporal

public:
  /**
   * @brief Constructor de nuestra simumlacion.
  */
  Simulation(/* args */);

  /**
   * @brief Destructor de nuestra simumlacion.
  */  
  ~Simulation();

  /**
   * @brief Metodo que se encarga de dirgir nuestra simulacion.
   * @param char plate[20]
   * @param double pTime
   * @param double pDifusividad
   * @param double h
   * @param double epsilon
   * @param string route
   * @param int thread_count
   * @return int
  */    
  int run(char plate[20], double pTime, double pDifusividad, double h,
  double epsilon, string route, int thread_count);

  /**
   * @brief Carga el archivo de trabajo con la direccion de nuestra plate
   * @param char plate[20]r.
   * @return vector<string>
  */  
  vector<string> load_job_file(char plate[20]);
  
  /**
   * @brief Lee el archivo binario con el que se encargara de llenara la matriz
   * @param string troute, la ruta del archivo a leer.
   * @return vector<vector<double>>, la matriz producto de leer el archivo.
  */
  vector<vector<double>> read_bin_file(/*char plate[]*/string troute);

  /**
   * @brief Devuelve el tiempo de ejecucion
   * @return double
  */
  double cpu_time();

  /**
   * @brief Metodo para convertir en string el buffer del time stamp generado
   * @param char* a, hilera de caracteres a convertir
   * @param int size, size de la hilera.
   * @return string, La hilera de caracteres en string.
  */  
  string convertToString(char* a, int size);

  /**
   * @brief Impresión de fecha y hora en la que se corre el código
   * @return string
  */
  string timestamp();

  /**
   * @brief Imprime en el archivo output la salida del la matriz de adyacencia
   *        procesada.
   * @param char output_filename[80]r
   * @param string stime
   * @param string stamp
   * @param vector<string> vectJob
   * @return void
  */    
  void printOutput(char output_filename[80], string stime,
    string stamp, vector<string> vectJob);
  
  /**
   * @brief Metodo fuer en el que se generan los calculos de calor para 
   *        posteriormente producir una matriz resultado, en la que se 
   *        encuentran los valor pertenecientes a la lamina.
   * @param double epsilon
   * @param double diff
   * @param double pTime
   * @param double pDifusividad
   * @param int thread_count
   * @param double h
   * @return void
  */     
  void calculateHeatTransfer(double epsilon, double diff,
    double pTime, double pDifusividad, int thread_count, double h);
};

