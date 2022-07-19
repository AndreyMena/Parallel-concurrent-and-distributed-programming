// Copyright 2022 ECCI-UCR

#include <mpi.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cstdint>
#include <cstdlib>

#include "FactApp.hpp"

FactApp::FactApp(int process_number, int process_count)
  : process_number(process_number)
  , process_count(process_count) {
}

void FactApp::run() {
  std::string file;
  std::vector<std::string> files;

  // TODO(you): distribute work
  (void)this->process_number;
  (void)this->process_count;

  size_t files_count = 0;

  FactDecomposer decomposer;
  // Read all filenames from stdin
  if (process_number == 0) {
    while (std::getline(std::cin, file)) {
      files.push_back(file);
    }
    files_count = files.size();

    for (int target = 1; target < process_count; ++target) {
      static_assert(sizeof(files_count) == sizeof(uint64_t));
      if (MPI_Send(&files_count, /*count*/ 1, MPI_UINT64_T, target
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fail("could not send file count");
      }
      if (MPI_Send(&files[0], files_count, MPI_CHAR, target
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fail("could not send files");
      }
    }

    for (size_t index = 0; index < files.size(); ++index) {
      // TODO(you): send files to processes, Decomposers receive them
      if (MPI_Send(&index, /*count*/ 1, MPI_UINT64_T, 0
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fail("could not send file count");
      }      
      if (MPI_Send(&files[index], 1, MPI_CHAR, 0
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fail("could not send files");
      }
    }

    int64_t stopCondition = 0;
    for (int target = 1; target < process_count; ++target) {
      if (MPI_Send(&stopCondition, /*count*/ 1, MPI_INT64_T, 0
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fail("could not send file count");
      }  
    }    
  } else {
    if (MPI_Recv(&files_count, /*capacity*/ 1, MPI_UINT64_T, /*source*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("could not receive file count");
    }

    file.resize(files_count);

    if (MPI_Recv(&files[0], /*capacity*/ files_count, MPI_CHAR, /*source*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("could not receive files");
    }

    uint64_t stopCondition = 1; 
    uint64_t index = -1;
    while (stopCondition != 0) {
      if (MPI_Recv(&index, /*capacity*/ 1, MPI_UINT64_T, /*source*/ 0
        , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
        fail("could not receive file count");
      }
      stopCondition = index;
      if (MPI_Recv(&files[index], /*capacity*/ 1, MPI_CHAR, /*source*/ 0
        , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
        fail("could not receive files");
      }
      if (stopCondition != -1) {
        decomposer.processFile(index, files[index]);
      }
      if (MPI_Recv(&stopCondition, /*capacity*/ 1, MPI_UINT64_T, /*source*/ 0
        , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
        fail("could not receive file count");
      }      
    }      
  }


  // TODO(you): Receive reports from Reporters and print them
  for (size_t index = 0; index < files.size(); ++index) {
    std::cout << files[index] << std::endl;
  }
}
