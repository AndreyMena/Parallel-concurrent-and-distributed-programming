#include <cstdlib>
#include <iostream>
#include <omp.h>

int main(int argc, char * argv[]){
  int thread_count = omp_get_max_threads();

  if (argc == 2){
    thread_count = atoi(argv[1]);
  }
  std::cout << "Hello from main thread\n";

  #pragma omp parallel num_threads(thread_count)
  {
    #pragma omp critical(ejemplo)
    std::cout << "Hello from secondary thread " << omp_get_thread_num()
    << "/" << omp_get_num_threads() << std::endl;
  }
}