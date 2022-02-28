#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min(a,b) (((a)<(b))?(a):(b))
#define NR_LETTERS 26

pthread_barrier_t pthread_barrier;
pthread_mutex_t mutex;

int L;
char **files;
int letters[NR_LETTERS];
int nr_of_threads;
int nr_of_files;

void *f(void *arg) {
    int thread_id = *(int *)arg;

    int start = thread_id * (double)nr_of_files / nr_of_threads;
    int end = min((thread_id + 1) * (double)nr_of_files / nr_of_threads, nr_of_files);
    
    for (int i = start; i < end; i++) {
        FILE *file = NULL;
        unsigned char buffer[L];
        size_t bytesRead = 0;

        file = fopen(files[i], "rb");   

        if (file != NULL) {
            while ((bytesRead = fread(buffer, 1, L, file)) > 0) {
                printf("%s \n", buffer);
                for(int j = 0; j < L; j++) {
                    if ( buffer[j] - 65 <= 25 && buffer[j] - 65 >= 0) {
                        pthread_mutex_lock(&mutex);
                        letters[buffer[j] - 65] += 1;
                        pthread_mutex_unlock(&mutex);
                    }
                    if ( buffer[j] - 97 <= 25 && buffer[j] - 97 >= 0) {
                        pthread_mutex_lock(&mutex);
                        letters[buffer[j] - 97] += 1;
                        pthread_mutex_unlock(&mutex);
                    }

                }
                memset(buffer, 0, sizeof(char) * L);
            }
        }
        fclose(file); 
    }

    
    pthread_barrier_wait(&pthread_barrier);
    if (thread_id == 0) {
        for(int i = 0; i < NR_LETTERS; i++) {
            char c = i + 65;
            printf("Caracterul %c apare de %d \n", c, letters[i]);
        }
    }

	pthread_exit(NULL);
}



int main(int argc, char *argv[]) {

    L = atoi(argv[1]);
    nr_of_threads = atoi(argv[2]);
    nr_of_files = atoi(argv[3]);

    files  = calloc(nr_of_files, sizeof(char *));
    for(int i = 0; i < nr_of_files; i++) {
        files[i] = calloc(24, sizeof(char));
	}


    for (int i = 0; i < nr_of_files; i++) {
        char *file;
        scanf("%s", file);
        strcpy(files[i], file);
    }

    printf("Se va citi din urmatoarel fisiere:\n");
    for (int i = 0; i < nr_of_files; i++) {
        printf("%s \n", files[i]);
    }

	pthread_t threads[nr_of_threads];
  	int r;
    int thread_id[nr_of_threads];

    r =  pthread_barrier_init(&pthread_barrier, NULL, nr_of_threads);
	if (r) {
		printf("Eroare la crearea barierei \n");
		exit(-1);
	}

    r = pthread_mutex_init(&mutex, NULL);

	if (r) {
		printf("Eroare la initializarea mutex-ului \n");
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

    r = pthread_mutex_destroy(&mutex);

	if (r) {
		printf("Eroare la distrugerea mutex-ului \n");
		exit(-1);
	}

  	pthread_exit(NULL);
}
