#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct _structs {
    individual **arr;
    individual **current_generation;
	individual **next_generation;
	individual *output;
    pthread_barrier_t *pthread_barrier;
    const sack_object *objects;
    int object_count;
    int generations_count;
    int sack_capacity;
    int thread_id;
    int nr_threads;
    int nr_per_thread;
    int *max_values_from_thread;
} thread_arg;

#endif