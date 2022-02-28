#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MASTER 0

int main (int argc, char *argv[]) {

    int N;
    int X;
    int *arr;
    int start;
    int end;
    int num_app = 0;

    int numtasks, rank;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    MPI_Status status;

    if (rank == MASTER) {
        scanf("%d", &X);

        scanf("%d", &N);
        while (N % numtasks != 0) {
            printf("N nu este divizibil cu numarul de procese! Reintroduce N!\n ");
            scanf("%d", &N);
        }

        arr = (int *)malloc(N * sizeof(int));
        for (int i = 0; i < N; i++) {
            scanf("%d", &arr[i]);
        }

        for (int i = 1; i < numtasks; i++) {

            MPI_Send(&X, 1, MPI_INT, i, 0, MPI_COMM_WORLD);

            start = i * (double)N / numtasks;
            end = MIN((i + 1) * (double)N / numtasks, N);
            int vec_dim = (end - start);
            MPI_Send(&vec_dim, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            MPI_Send(arr + start, vec_dim, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        
        start = rank * (double)N / numtasks;
        end = MIN((rank + 1) * (double)N / numtasks, N);

        for (int i = start; i < end; i++) {
            if(X == arr[i]) {
                num_app++;
            }
        }

        int local_app;
        for (int i = 1; i < numtasks; i++) {
            MPI_Recv(&local_app, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
            num_app += local_app;
        }

        printf("Numarul de aparitii a lui %d este de: %d \n", X, num_app);

    } else {
        MPI_Recv(&X, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);

        MPI_Recv(&N, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
        arr = (int *)malloc(N * sizeof(int));
        MPI_Recv(arr, N, MPI_INT, MASTER, 0, MPI_COMM_WORLD, &status);
        
        for (int i = 0; i < N; i++) {
            if(X == arr[i]) {
                num_app++;
            }
        }

        MPI_Send(&num_app, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

    }

    MPI_Finalize();

}