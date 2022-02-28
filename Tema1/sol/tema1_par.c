#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "genetic_algorithm.h"

int main(int argc, char *argv[]) {
	// array with all the objects that can be placed in the sack
	sack_object *objects = NULL;
	// barrier that will hellp us to get all threads alined
	pthread_barrier_t pthread_barrier;
	// number of objects
	int object_count = 0;
	// maximum weight that can be carried in the sack
	int sack_capacity = 0;
	// number of generations
	int generations_count = 0;
	/* variable that will help us know if threads/barrier/mutex were created
	without problems */
	int r;
	/* number of threads */
	int nr_threads = atoi(argv[3]);

	r =  pthread_barrier_init(&pthread_barrier, NULL, nr_threads);

	if (r) {
		printf("Eroare la crearea barierei \n");
		exit(-1);
	}

	if (!read_input(&objects, &object_count, &sack_capacity, &generations_count, argc, argv)) {
		return 0;
	}
	
	pthread_t *threads = (pthread_t*) calloc(nr_threads, sizeof(pthread_t));
	thread_arg *arguments = (thread_arg *) calloc(nr_threads, sizeof(thread_arg));
	individual *current_generation = (individual*) calloc(object_count, sizeof(individual));
	individual *next_generation = (individual*) calloc(object_count, sizeof(individual));
	individual *output = (individual*) calloc(object_count, sizeof(individual));
	individual ** arr = calloc(nr_threads, sizeof(individual *));
	int *max_values_from_thread = (int*) calloc(nr_threads, sizeof(int));

    for(int i = 0; i < nr_threads; i++) {
        arr[i] = calloc((int)ceil((double)object_count/nr_threads), sizeof(individual));
	}

	/* set initial generation (composed of object_count individuals with a single item in the sack) */
	for (int i = 0; i < object_count; ++i) {
		current_generation[i].fitness = 0;
		current_generation[i].chromosomes = (int*) calloc(object_count, sizeof(int));
		current_generation[i].chromosomes[i] = 1;
		current_generation[i].index = i;
		current_generation[i].chromosome_length = object_count;

		next_generation[i].fitness = 0;
		next_generation[i].chromosomes = (int*) calloc(object_count, sizeof(int));
		next_generation[i].index = i;
		next_generation[i].chromosome_length = object_count;
	}

	/* creating the threads */
	for (int i = 0; i < nr_threads; i++) {
		arguments[i].arr = arr;
		arguments[i].current_generation = &current_generation;
		arguments[i].next_generation = &next_generation;
		arguments[i].output = output;
		arguments[i].pthread_barrier = &pthread_barrier;
		arguments[i].objects = objects;
		arguments[i].object_count = object_count;
		arguments[i].generations_count = generations_count;
		arguments[i].sack_capacity = sack_capacity;
		arguments[i].thread_id = i;
		arguments[i].nr_threads = nr_threads;
		arguments[i].nr_per_thread = (int)ceil((double)object_count/nr_threads);
		arguments[i].max_values_from_thread = max_values_from_thread;

		r = pthread_create(&threads[i], NULL, run_genetic_algorithm_par, &arguments[i]);
		
		if (r) {
			printf("Eroare la crearea thread-ului %d\n", i);
			exit(-1);
		}
	}

	// joining all the threads
	for (int i = 0; i < nr_threads; i++) {
		r = pthread_join(threads[i], NULL);

		if (r) {
			printf("Eroare la asteptarea thread-ului %d\n", i);
			exit(-1);
		}
	}

	r = pthread_barrier_destroy(&pthread_barrier);

	if (r) {
		printf("Eroare la distrugerea barierei \n");
		exit(-1);
	}

	free(objects);
	free(max_values_from_thread);
	free(threads);
	free_generation(current_generation);
	free_generation(next_generation);
	free(arguments);
	
	return 0;
}
