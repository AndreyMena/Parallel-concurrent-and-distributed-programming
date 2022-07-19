// Copyright 2021 NULL-POINTERS Universidad de Costa Rica. CC BY 4.0
#include <math.h>
#include <unistd.h>

#include <fstream>
#include <iostream>
#include <string>

/**
   @brief Compara dos placas binarias y muestra en pantalla si son iguales o
   diferentes
   @param plate01 es el nombre de la primera placa a comparar
   @param plate02 es el nombre de la segunda placa a comparar
   @param epsilon es el valor que se tomara de referencia en la resta para
   comparar
   @return un int si la salida es correcta o incorrecta
  */
int comparar(std::string plate01, std::string plate02, double epsilon);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  std::string plate1;
  std::string plate2;
  std::string epsilonS;  // get_current_dir_name();
  double epsilon;
  if (argc == 4) {
    plate1 = std::string(argv[1]);
    plate2 = std::string(argv[2]);
    epsilonS = std::string(argv[3]);
    try {
      epsilon = std::stod(epsilonS);
    } catch (...) {
      fprintf(stderr,
              "Error: El epsilon ingresado no es del formato correcto\n");
    }

  } else {
    fprintf(stderr,
            "Error: Debe agregar el nombre del archivo por parametro\n");
    return EXIT_FAILURE;
  }

  error = comparar(plate1, plate2, epsilon);
  return error;
}

int comparar(std::string plate01, std::string plate02, double e) {
  std::ifstream plate01File(plate01, std::fstream::in | std::fstream::binary);
  std::ifstream plate02File(plate02, std::fstream::in | std::fstream::binary);
  if (!plate01File && !plate02File) {
    std::cout << "No se puede abrir el binario." << std::endl;
    return false;
  } else {
    uint64_t filas1, filas2, columnas1, columnas2;

    // Se lee filas
    plate01File.read(reinterpret_cast<char*>(&filas1), sizeof(filas1));
    plate02File.read(reinterpret_cast<char*>(&filas2), sizeof(filas2));
    // Se lee columnas
    plate01File.read(reinterpret_cast<char*>(&columnas1), sizeof(columnas1));
    plate02File.read(reinterpret_cast<char*>(&columnas2), sizeof(columnas2));
    double valorCeldaPlaca1;
    double valorCeldaPlaca2;
    if ((filas1 == filas2) && (columnas1 == columnas2)) {
      for (uint64_t fila = 0; fila < filas1; fila++) {
        for (uint64_t columna = 0; columna < columnas1; columna++) {
          plate01File.read(reinterpret_cast<char*>(&valorCeldaPlaca1),
                           sizeof(valorCeldaPlaca1));
          plate02File.read(reinterpret_cast<char*>(&valorCeldaPlaca2),
                           sizeof(valorCeldaPlaca2));
          if (fabs(valorCeldaPlaca1 - valorCeldaPlaca2) > e) {
            std::cout << "Las laminas son diferentes" << std::endl;
            plate01File.close();
            plate02File.close();
            return 0;
          }
        }
      }
      std::cout << "Las laminas son iguales" << std::endl;
    } else {
      std::cout << "Las laminas son diferentes" << std::endl;
    }
    plate01File.close();
    plate02File.close();
  }
  return 0;
}
