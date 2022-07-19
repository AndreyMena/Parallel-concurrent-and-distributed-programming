/*
 * Copyright 2022 Gollo
 */
#include <cstring>
#include <ctime>
#include <string>
#include <vector>

using namespace std;  // NOLINT

class Simulation {
 private:
  /* data */
  vector<vector<double>> orgn_mtx;  // matriz original
  vector<vector<double>> temp_mtx;  // matriz temporal

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
   * @brief Metodo que se encarga de enviar las unidades
   *        de trabajo dinamicamente
   * @param process_number int&
   * @param process_count int&
   * @param thread_count int&
   * @param route string&
   * @param outputJobTsv ofstream&
   * @param jobs ifstream&
   * @param vectPlates vector<string>&
   * @param vectPtimes vector<double>&
   * @param vectPdifusividad vector<double>&
   * @param vecth route
   * @param vecteEpsilon thread_count
   * @param lineas
   * @param orderIterations
   * @return int
   */
int sendDynamic(int& process_number, int& process_count,
  int& thread_count, string& route, ofstream& outputJobTsv,
  ifstream& jobs, vector<string>& vectPlates, vector<double>& vectPtimes,
  vector<double>& vectPdifusividad, vector<double>& vecth,
  vector<double>& vecteEpsilon, int& lineas, vector<int>& orderIterations);



  /**
   * @brief Metodo que se encarga de recibir dinamicamente los
   *        datos.
   * @param process_number int
   * @param process_count int
   * @return int
   */
int receiveDynamic(int process_number, int process_count);

  /**
   * @brief Metodo que se encarga de dirgir nuestra simulacion.
   * @param plate char*
   * @param pTime double
   * @param pDifusividad double
   * @param h double
   * @param epsilon double
   * @param route string
   * @param thread_count int
   * @param process_number int
   * @return int
   */
  int run(char plate[20], double pTime, double pDifusividad, double h,
          double epsilon, string route, int thread_count
          , int process_number);

  /**
   * @brief Carga el archivo de trabajo con la direccion de nuestra plate
   * @param plate char
   * @return vector<string>
   */
  vector<string> load_job_file(char plate[20]);

  /**
   * @brief Lee el archivo binario con el que se encargara de llenara la matriz
   * @param troute string, la ruta del archivo a leer.
   * @param fila uint64_t, la ruta del archivo a leer.
   * @param columna uint64_t, la ruta del archivo a leer.
   * @return vector<vector<double>>, la matriz producto de leer el archivo.
   */
  vector<vector<double>> read_bin_file(string troute, uint64_t& fila,
                                       uint64_t& columna);

  /**
   * @brief Imprime en el archivo output la salida del la matriz de adyacencia
   *        procesada.
   * @param output_filename char
   * @param fila uint64_t
   * @param columna uint64_t
   * @param iterations int
   * @return void
   */
  void printOutput(char output_filename[80], uint64_t fila, uint64_t columna,
                   int iterations);

  /**
   * @brief Calcula el tiempo que sera coloca en la salida.
   * @param secondsTime double
   * @return void
   */
  stringstream calculateTime(double secondsTime);

  /**
   * @brief Metodo fuer en el que se generan los calculos de calor para
   *        posteriormente producir una matriz resultado, en la que se
   *        encuentran los valor pertenecientes a la lamina.
   * @param epsilon double
   * @param diff double
   * @param pTime double
   * @param pDifusividad double
   * @param thread_count double
   * @param h double
   * @param iterations int
   * @return void
   */
  void calculateHeatTransfer(double epsilon, double diff, double pTime,
                             double pDifusividad, int thread_count, double h,
                             int& iterations);
};
