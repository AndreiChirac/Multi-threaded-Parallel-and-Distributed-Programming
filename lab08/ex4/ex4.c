#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ROOT 3

int main (int argc, char *argv[])
{
    int  numtasks, rank, len;
    char hostname[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Get_processor_name(hostname, &len);

    int value;
    MPI_Status status;

    if (rank == ROOT) {

        MPI_Recv(&value, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        // The ROOT process receives an element from any source.
        // Prints the element and the source. HINT: MPI_Status.
        printf("Nr: %d, source : %d \n", value ,status.MPI_SOURCE);

    } else {

        // Generate a random number.
        srand(time(NULL));
        value = rand() % (rank * 50 + 1);

        printf("Process [%d] send %d.\n", rank, value);
        MPI_Send(&value, 1, MPI_INT, ROOT, rank, MPI_COMM_WORLD);
        // Sends the value to the ROOT process.

    }

    MPI_Finalize();

}

