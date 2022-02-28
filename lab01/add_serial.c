#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>


#define min(a,b) (((a)<(b))?(a):(b))
#define NUM_THREADS 4
/*
    schelet pentru exercitiul 5
*/

int* arr;
int array_size;

struct pair_start_end {
    int start;
    int end;
};

void *f(void *arg) {
  	struct pair_start_end start_end  = *(struct pair_start_end*)arg;

    for (int i = start_end.start; i < start_end.end; i++) {
        arr[i] += 100;
    }

	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    if (argc < 2) {
        perror("Specificati dimensiunea array-ului\n");
        exit(-1);
    }

    array_size = atoi(argv[1]);
    
    arr = malloc(array_size * sizeof(int));
    for (int i = 0; i < array_size; i++) {
        arr[i] = i;
    }

    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }

	pthread_t threads[NUM_THREADS];
  	int r;
  	long id;
  	void *status;
	struct pair_start_end idx[NUM_THREADS];

  	for (id = 0; id < NUM_THREADS; id++) {
          
        int start = id * (double)array_size / NUM_THREADS;
        int end = min((id + 1) * (double)array_size / NUM_THREADS, array_size);

        idx[id].start = start;
        idx[id].end = end;

		r = pthread_create(&threads[id], NULL, f, &idx[id]);

		if (r) {
	  		printf("Eroare la crearea thread-ului %ld\n", id);
	  		exit(-1);
		}
        
  	}

  	for (id = 0; id < NUM_THREADS; id++) {
		r = pthread_join(threads[id], &status);

		if (r) {
	  		printf("Eroare la asteptarea thread-ului %ld\n", id);
	  		exit(-1);
		}
  	}

    for (int i = 0; i < array_size; i++) {
        printf("%d", arr[i]);
        if (i != array_size - 1) {
            printf(" ");
        } else {
            printf("\n");
        }
    }

  	pthread_exit(NULL);
}
