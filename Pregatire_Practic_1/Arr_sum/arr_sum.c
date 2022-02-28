#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define min(a,b) (((a)<(b))?(a):(b))


pthread_barrier_t pthread_barrier;

int *arr_1;
int *arr_2;
int array_size_1;
int array_size_2;
int array_size;
int nr_of_threads;

void *f(void *arg) {
    int thread_id = *(int *)arg;

    int start = thread_id * (double)array_size / nr_of_threads;
    int end = min((thread_id + 1) * (double)array_size / nr_of_threads, array_size);
    
    for (int i = start; i < end; i++) {
        if (array_size_1 < array_size_2) {
            arr_2[i] += arr_1[i];
        } else {
            arr_1[i] += arr_2[i];
        }
    }
    pthread_barrier_wait(&pthread_barrier);
    
    if (thread_id == 0) {
        if (array_size_1 < array_size_2) {
            for (int i = 0; i < array_size_2; i++) {
                printf("%d ", arr_2[i]);
            }
        } else {
            for (int i = 0; i < array_size_1; i++) {
                printf("%d ", arr_1[i]);
            }
        }
    }
    
	pthread_exit(NULL);
}



int main(int argc, char *argv[]) {

    array_size_1= atoi(argv[1]);
    array_size_2= atoi(argv[2]);

    if (array_size_1 > array_size_2) {
        array_size = array_size_2;
    } else {
        array_size = array_size_1;
    }

    nr_of_threads = atoi(argv[3]);

    arr_1 = malloc(array_size_1 * sizeof(int));
    arr_2 = malloc(array_size_2 * sizeof(int));

    for (int i = 0; i < array_size_1; i++) {
        scanf("%d", &arr_1[i]);
    }

    for (int i = 0; i < array_size_2; i++) {
        scanf("%d", &arr_2[i]);
    }

	pthread_t threads[nr_of_threads];
  	int r;
    int thread_id[nr_of_threads];

    r =  pthread_barrier_init(&pthread_barrier, NULL, nr_of_threads);
	if (r) {
		printf("Eroare la crearea barierei \n");
		exit(-1);
	}

  	for (int id = 0; id < nr_of_threads; id++) {

        thread_id[id] = id;

		r = pthread_create(&threads[id], NULL, f, &thread_id[id]);

		if (r) {
	  		printf("Eroare la crearea thread-ului %d\n", id);
	  		exit(-1);
		}
        
  	}

  	for (int id = 0; id < nr_of_threads; id++) {
		r = pthread_join(threads[id], NULL);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %d\n", id);
	  		exit(-1);
		}
  	}

    r = pthread_barrier_destroy(&pthread_barrier);

	if (r) {
		printf("Eroare la distrugerea barierei \n");
		exit(-1);
	}

  	pthread_exit(NULL);
}
