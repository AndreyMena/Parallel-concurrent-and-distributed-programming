/*
 * Copyright 2021 Jeisson Hidalgo-Cespedes - Universidad de Costa Rica
 * Creates a secondary thread that greets in the standard output
 */

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct shared_thread_data {
  size_t team_count;
  useconds_t stage1_delay;
  useconds_t stage2_delay;
  //pthread_barrier_t start_barrier;
  sem_t* batons;
  pthread_mutex_t position_mutex;
  size_t position;
  pthread_mutex_t stdout_mutex;
  size_t runners_ready;
  pthread_cond_t can_begin_stage1;
  pthread_mutex_t can_access_runners;
} shared_thread_data_t;

typedef struct private_thread_data {
  size_t thread_number;  // rank
  shared_thread_data_t* shared_thread_data;
} private_thread_data_t;

int create_threads(shared_thread_data_t* shared_thread_data);
int analyze_arguments(int argc, char* argv[]
    , shared_thread_data_t* shared_data);
void* start_race(void* data);
void* finish_race(void* data);


int main(int argc, char* argv[]) {
  int error = 0;

  shared_thread_data_t* shared_thread_data = (shared_thread_data_t*)
    calloc(1, sizeof(shared_thread_data_t));

  if (shared_thread_data) {
    error = analyze_arguments(argc, argv, shared_thread_data);
    if (error == 0 ) {
      shared_thread_data->position = 0;
      //error += pthread_barrier_init(&shared_thread_data->start_barrier
      //  , /*attr*/ NULL, /*count*/ shared_thread_data->team_count);
      error += pthread_mutex_init(&shared_thread_data->position_mutex, NULL);
      error += pthread_mutex_init(&shared_thread_data->stdout_mutex, NULL);
      error += pthread_mutex_init(&shared_thread_data->can_access_runners, NULL);
      error += pthread_cond_init(&shared_thread_data->can_begin_stage1, /*attr*/ NULL);

      shared_thread_data->batons = calloc(shared_thread_data->team_count, 
        sizeof(sem_t));
      shared_thread_data->runners_ready = 0;  

      if (error == 0 && shared_thread_data->batons) {
        for (size_t index = 0; index < shared_thread_data->team_count; ++index) {
          sem_init(&shared_thread_data->batons[index], 0, 0);
        }

        struct timespec start_time, finish_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

        error = create_threads(shared_thread_data);

        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);
        double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        printf("execution time: %.9lfs\n", elapsed_time);

        for (size_t index = 0; index < shared_thread_data->team_count; ++index) {
          sem_destroy(&shared_thread_data->batons[index]);
        }

        free(shared_thread_data->batons);
        //pthread_barrier_destroy(&shared_thread_data->start_barrier);
        pthread_mutex_destroy(&shared_thread_data->position_mutex);
        pthread_mutex_destroy(&shared_thread_data->stdout_mutex);
        pthread_cond_destroy(&shared_thread_data->can_begin_stage1);
        pthread_mutex_destroy(&shared_thread_data->can_access_runners);
      } else {
        fprintf(stderr, "error: could not init mutex\n");
        error = 11;
      }
    }

    free(shared_thread_data);
  } else {
    fprintf(stderr, "error: could not allocated shared memory\n");
    error = 12;
  }

  return error;
}

int analyze_arguments(int argc, char* argv[]
    , shared_thread_data_t* shared_data) {
	if (argc != 4) {
		fprintf(stderr
      , "usage: relay_race teams stage1_duration stage2_duration\n");
		return 1;
	}

	if ( sscanf(argv[1], "%zu", &shared_data->team_count) != 1
     || shared_data->team_count == 0 ) {
		return (void)fprintf(stderr, "invalid team count: %s\n", argv[1]), 1;
  }

	if ( sscanf(argv[2], "%u", &shared_data->stage1_delay) != 1 ) {
		return (void)fprintf(stderr, "invalid stage 1 delay: %s\n", argv[2]), 2;
  }

	if ( sscanf(argv[3], "%u", &shared_data->stage2_delay) != 1 ) {
		return (void)fprintf(stderr, "invalid stage 2 delay: %s\n", argv[3]), 3;
  }

	return EXIT_SUCCESS;
}

int create_threads(shared_thread_data_t* shared_thread_data) {
  int error = 0;

  const size_t thread_count = 2 * shared_thread_data->team_count;
  pthread_t* threads = (pthread_t*)
    malloc(thread_count * sizeof(pthread_t));

  private_thread_data_t* private_thread_data = (private_thread_data_t*)
    calloc(thread_count, sizeof(private_thread_data_t));

  if (threads && private_thread_data) {
    for (size_t index = 0; index < shared_thread_data->team_count; ++index) {
      private_thread_data[index].thread_number = index;
      private_thread_data[index].shared_thread_data = shared_thread_data;

      error = pthread_create(&threads[index], NULL, start_race
        , &private_thread_data[index]);

      if (error) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }

    for (size_t index = shared_thread_data->team_count; index < thread_count;
        ++index) {
      private_thread_data[index].thread_number = index;
      private_thread_data[index].shared_thread_data = shared_thread_data;

      error = pthread_create(&threads[index], NULL, finish_race
        , &private_thread_data[index]);

      if (error) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }

    for (size_t index = 0; index < thread_count; ++index) {
      pthread_join(threads[index], NULL);
    }

    free(private_thread_data);
    free(threads);
  } else {
    fprintf(stderr, "error: could not allocate memory for %zu threads\n"
      , shared_thread_data->team_count);
    error = 22;
  }

  return error;
}

void* start_race(void* data) {
  private_thread_data_t* private_data = (private_thread_data_t*)data;
  shared_thread_data_t* shared_data = private_data->shared_thread_data;

  //pthread_barrier_wait(&shared_data->start_barrier);
  pthread_mutex_lock(&shared_data->can_access_runners);
 
  ++shared_data->runners_ready;
  if (shared_data->runners_ready == shared_data->team_count){
    pthread_cond_broadcast(&shared_data->can_begin_stage1);
  }else {
    while (pthread_cond_wait(&shared_data->can_begin_stage1, &shared_data->can_access_runners) != 0);
    //pthread_cond_signal(&shared_data->can_begin_stage1);
  }
  pthread_mutex_unlock(&shared_data->can_access_runners);


  usleep(1000 * shared_data->stage1_delay);
  sem_post(&shared_data->batons[private_data->thread_number]);

  return NULL;
}

void* finish_race(void* data) {
  private_thread_data_t* private_data = (private_thread_data_t*)data;
  shared_thread_data_t* shared_data = private_data->shared_thread_data;

  const size_t team_number = private_data->thread_number
    - shared_data->team_count;
  assert(team_number < shared_data->team_count);

  sem_wait(&shared_data->batons[team_number]);
  usleep(1000 * shared_data->stage2_delay);

  pthread_mutex_lock(&private_data->shared_thread_data->position_mutex);
  const size_t our_position = ++shared_data->position;
  printf("Place %zu: team %zu\n", our_position, team_number);
  pthread_mutex_unlock(&private_data->shared_thread_data->position_mutex);

//  pthread_mutex_lock(&private_data->shared_thread_data->stdout_mutex);
//  printf("Place %zu: team %zu\n", our_position, team_number);
//  pthread_mutex_unlock(&shared_data->stdout_mutex);

  return NULL;
}
