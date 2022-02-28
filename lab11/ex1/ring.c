#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[])
{
    int numtasks, rank;
    int flag;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int recv_num;
    MPI_Status status;
    MPI_Request request;

    // First process starts the circle.
    if (rank == 0) {
        recv_num = rand() % 20;
        printf("%d \n", recv_num);
        MPI_Isend(&recv_num, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, &request);
        MPI_Test(&request, &flag, &status);
        if (flag) {
            printf("[P0] The send operation is over\n");
        } else {
            printf("[P0] The send operation is not over yet\n");
            MPI_Wait(&request, &status);
        }

        MPI_Irecv(&recv_num, 1, MPI_INT, numtasks - 1, 0, MPI_COMM_WORLD, &request);
        MPI_Test(&request, &flag, &status);
        if (flag) {
            printf("[P%d] The send operation is over\n", rank);
        } else {
            printf("[P%d] The send operation is not over yet\n", rank);
            MPI_Wait(&request, &status);
        }
        printf("%d \n", recv_num);
        // First process starts the circle.
        // Generate a random number.
        // Send the number to the next process.

    } else if (rank == numtasks - 1) {
        MPI_Irecv(&recv_num, 1, MPI_INT, numtasks - 2, 0, MPI_COMM_WORLD, &request);
        MPI_Test(&request, &flag, &status);
        if (flag) {
            printf("[P%d] The send operation is over\n", rank);
        } else {
            printf("[P%d] The send operation is not over yet\n", rank);
            MPI_Wait(&request, &status);
        }

        recv_num += 2;

        MPI_Isend(&recv_num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &request);
        MPI_Test(&request, &flag, &status);
        if (flag) {
            printf("[P%d] The send operation is over\n", rank);
        } else {
            printf("[P%d] The send operation is not over yet\n", rank);
            MPI_Wait(&request, &status);
        }
        // Last process close the circle.
        // Receives the number from the previous process.
        // Increments the number.
        // Sends the number to the first process.
        
    } else {

        MPI_Irecv(&recv_num, 1, MPI_INT, rank - 1, 0, MPI_COMM_WORLD, &request);
        MPI_Test(&request, &flag, &status);
        if (flag) {
            printf("[P%d] The send operation is over\n", rank);
        } else {
            printf("[P%d] The send operation is not over yet\n", rank);
            MPI_Wait(&request, &status);
        }

        recv_num += 2;

        MPI_Isend(&recv_num, 1, MPI_INT, rank + 1, 0, MPI_COMM_WORLD, &request);
        MPI_Test(&request, &flag, &status);
        if (flag) {
            printf("[P%d] The send operation is over\n", rank);
        } else {
            printf("[P%d] The send operation is not over yet\n", rank);
            MPI_Wait(&request, &status);
        }
        // Middle process.
        // Receives the number from the previous process.
        // Increments the number.
        // Sends the number to the next process.

    }

    MPI_Finalize();

}

