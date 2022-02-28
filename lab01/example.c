#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>


//#define NUM_THREADS 2

// EX1 - SCHIMBAREA NUMARULUI DE THRED-URI
#define NUM_THREADS 4

// Ex3 - Mesajele sunt afisate intr-o ordine aleatorie

/*
void *f(void *arg) {
  	long id = *(long*)arg;

	for (size_t i = 0; i < 100; i++) {
		printf("Hello World din thread-ul %ld! Apartine core-ului %ld\n", id, i);
  	}

	pthread_exit(NULL);
}
*/

void *f(void *arg) {
  	long id = *(long*)arg;

	printf("Hello World din thread-ul %ld!\n", id);

	pthread_exit(NULL);
}

void *g(void *arg) {
  	long id = *(long*)arg;

	printf("Goodbye World din thread-ul %ld!\n", id);

	pthread_exit(NULL);
}


int main(int argc, char *argv[]) {
	
	// Numarul de thred-uri egal cu numarul de core-uri
	long cores = sysconf(_SC_NPROCESSORS_CONF);

	pthread_t threads[cores];
  	int r;
  	long id;
  	void *status;
	long ids[cores];

  	for (id = 0; id < cores; id++) {
		ids[id] = id; 
		r = pthread_create(&threads[id], NULL, f, &ids[id]);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}

		// Thred pentru functia g
		r = pthread_create(&threads[id], NULL, g, &ids[id]);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}

  	}

  	for (id = 0; id < cores; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

  	pthread_exit(NULL);
}
