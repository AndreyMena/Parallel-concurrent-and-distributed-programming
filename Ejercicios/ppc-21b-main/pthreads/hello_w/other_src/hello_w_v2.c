// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>

int create_threads(uint64_t thread_count);

void* greet(void* data);

// procedure main
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // thread_count := integer(argv[1])
  // Create thread_count as the result of converting argv[1] to integer
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  // assert(argc == 2);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "error: invalid thread count\n");
      return EXIT_FAILURE;
    }
  }

  error = create_threads(thread_count);
  return error;
}  // end procedure


int create_threads(uint64_t thread_count) {
  int error = EXIT_SUCCESS;
  // create_thread(greet)
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
  if (threads) {
    // for thread_number := 0 to thread_count do
    for (uint64_t thread_number = 0; thread_number < thread_count; ++thread_number) {
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
        , /*arg*/ (void*)thread_number);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could no create secondary thread\n");
        break;
      }
    }

    // print("Hello from main thread")
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < thread_count; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    free(threads);
  } else {
    fprintf(stderr, "error: could not allocate %ld threads\n", thread_count);
  }
  return error;
}

// procedure greet
void* greet(void* data) {
  const uint64_t thread_number = (uint64_t) data;
  // print("Hello from secondary thread")
  printf("Hello from secondary thread %" PRIu64 "\n", thread_number);
  return NULL;
}  // end procedure
