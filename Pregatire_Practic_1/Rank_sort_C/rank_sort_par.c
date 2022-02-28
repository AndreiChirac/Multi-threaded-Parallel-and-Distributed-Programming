#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define min(a,b) (((a)<(b))?(a):(b))


pthread_barrier_t pthread_barrier;

int *arr;
int *arr_helper;
int *arr_cnt;
int array_size;
int nr_of_threads;

void *f(void *arg) {
    int thread_id = *(int *)arg;

    int start = thread_id * (double)array_size / nr_of_threads;
    int end = min((thread_id + 1) * (double)array_size / nr_of_threads, array_size);
    for (int i = start; i < end; i++) {
        for (int j = 0; j < array_size; j++){
            if (arr[j] < arr[i]) {
                arr_cnt[i] = arr_cnt[i] + 1;
            }
        }
    }
    pthread_barrier_wait(&pthread_barrier);
    
    if (thread_id == 0) {
        printf("Cnt for each elem :\n");
        for (int i = 0; i < array_size; i++) {
            printf("%d ", arr_cnt[i]);
        }
        printf("\n");
    }

    for (int i = start; i < end; i++) {
        arr[arr_cnt[i]] = arr_helper[i];
    }

    pthread_barrier_wait(&pthread_barrier);

    if (thread_id == 0) {
        printf("Sorted arr:\n");
        for (int i = 0; i < array_size; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

	pthread_exit(NULL);
}



int main(int argc, char *argv[]) {

    array_size = atoi(argv[1]);
    nr_of_threads = atoi(argv[2]);

    arr = malloc(array_size * sizeof(int));
    arr_helper = malloc(array_size * sizeof(int));
    arr_cnt = calloc(array_size, sizeof(int));
    for (int i = 0; i < array_size; i++) {
        arr[i] = array_size - i + 5;
        arr_helper[i] = arr[i];
    }

    for (int i = 0; i < array_size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

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
