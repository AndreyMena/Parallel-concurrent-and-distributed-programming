// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// thread_shared_data_t
typedef struct shared {
  uint64_t position;
  pthread_mutex_t can_access_position;
  uint64_t thread_count;
} shared_data_t;

// thread_private_data_t
typedef struct private {
  uint64_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

void* race(void* data);
int create_threads(shared_data_t* shared_data);

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

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->position = 0;
    error = pthread_mutex_init(&shared_data->can_access_position, /*attr*/NULL);
    if (error == EXIT_SUCCESS) {
      shared_data->thread_count = thread_count;

      struct timespec start_time, finish_time;
      clock_gettime(CLOCK_MONOTONIC, &start_time);

      error = create_threads(shared_data);

      clock_gettime(CLOCK_MONOTONIC, &finish_time);
      double elapsed_time = finish_time.tv_sec - start_time.tv_sec
        + (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

      printf("Execution time: %.9lfs\n", elapsed_time);

      pthread_mutex_destroy(&shared_data->can_access_position);
      free(shared_data);
    } else {
      fprintf(stderr, "error: could not init mutex\n");
      error = 12;
    }
  } else {
    fprintf(stderr, "error: could not allocate shared memory\n");
    error = 11;
  }
  return error;
}  // end procedure


int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // create_thread(greet)
  pthread_t* threads = (pthread_t*)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    // for thread_number := 0 to thread_count do
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, race
        , /*arg*/ &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could no create secondary thread\n");
        error = 22;
        break;
      }
    }

    // print("Hello from main thread")
    //printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "error: could not allocate %ld threads\n"
      , shared_data->thread_count);
    error = 21;
  }
  return error;
}

// procedure race
void* race(void* data) {
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;
  uint64_t my_position = 0;

  // lock(can_access_position)
  pthread_mutex_lock(&shared_data->can_access_position);
  // race condition/data race/condición de carrera:
  // modificación concurrente de memoria compartida
  // position := position + 1
  ++shared_data->position;
  // my_position := position
   my_position = shared_data->position;

  // unlock(can_access_position)
  pthread_mutex_unlock(&shared_data->can_access_position);

  // print("Hello from secondary thread")
  printf("Thread %" PRIu64 "/%" PRIu64 ": I arrived at position %" PRIu64 "\n"
    , private_data->thread_number, shared_data->thread_count, my_position);
  
  return NULL;
}  // end procedure


