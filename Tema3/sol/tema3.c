#include<mpi.h>
#include<stdio.h>
#include<stdlib.h>
#include <string.h>

#define FILE_NR 7
#define CLUSTER_NR 3
#define MIN(a,b) ((a) < (b) ? (a) : (b))

void share_to_subordinate(int rank, int **record, int **record_nr_workers) {
    // each cluster will share the data with each subordinated worker
    for (int x = 0; x < record_nr_workers[rank][0]; x++) {
        for (int i = 0; i < 3; i++) {
            MPI_Send(&record_nr_workers[i][0], 1, MPI_INT, record[rank][x], rank, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, record[rank][x]);
            fflush(stdout);

            for (int j = 0; j < record_nr_workers[i][0]; j++) {
                MPI_Send(&record[i][j], 1, MPI_INT, record[rank][x], rank, MPI_COMM_WORLD);
                printf("M(%d,%d)\n", rank, record[rank][x]);
                fflush(stdout);
            }
        }
    }
}

void print_cluster(int process, int **record, int **record_nr_workers) {
    printf("%d -> ", process);
    for (int i = 0; i < 3; i++) {
        printf("%d:", i);
        for (int j = 0; j < record_nr_workers[i][0]; j++) {
            if (j == record_nr_workers[i][0] - 1) {
                if (i == 2) {
                    printf("%d\n", record[i][j]);
                } else {
                    printf("%d ", record[i][j]);
                }
            } else {
                printf("%d,", record[i][j]);
            }
        }
    }
}

int main(int argc, char * argv[]) {
    
    int numtasks, rank;
    char file_name[12] = "cluster .txt";
    //char file_name[28] = "../tests/test2/cluster .txt";

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);

    int **record = (int **)malloc(CLUSTER_NR * sizeof(int *));
    int **record_nr_workers = (int **)malloc(CLUSTER_NR * sizeof(int *));
    for (int i = 0; i < CLUSTER_NR; i++) {
        record_nr_workers[i] = (int *)malloc(sizeof(int));
    }
    
    int master;
    int size = atoi(argv[1]);
    int *V = (int *)malloc(size * sizeof(int));
    MPI_Status status;

    if (rank == 0 || rank == 1 || rank == 2) {
        master = rank;

        file_name[FILE_NR] = rank + 48;
        //file_name[22] = rank + 48;

        //printf("%s \n", file_name);

        FILE *fptr;

        if ((fptr = fopen(file_name,"r")) == NULL){
            printf("Error! opening file");

            // program exits if the file pointer returns NULL.
            exit(1);
        }
        
        fscanf(fptr, "%d", &record_nr_workers[rank][0]);
        record[rank] = (int *)malloc(record_nr_workers[rank][0] * sizeof(int));

        for (int i = 0; i < record_nr_workers[rank][0]; i++) {    
            fscanf(fptr, "%d", &record[rank][i]);

            // sending to the workers who is the master
            MPI_Send(&master, 1, MPI_INT, record[rank][i], rank, MPI_COMM_WORLD);
            printf("M(%d,%d)\n", rank, record[rank][i]);
            fflush(stdout);
        }

        fclose(fptr);
    } else {
        MPI_Recv(&master, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    }

    // share the data that each cluster read with the other clusters
    if(rank == 0) { 

        // sending the clusters 1 and 2 how many subordinates cluster 0 has
        MPI_Send(&record_nr_workers[rank][0], 1, MPI_INT, 1, rank, MPI_COMM_WORLD);
        printf("M(0,1)\n");
        fflush(stdout);

        MPI_Send(&record_nr_workers[rank][0], 1, MPI_INT, 2, rank, MPI_COMM_WORLD);
        printf("M(0,2)\n");
        fflush(stdout);

        // sending the clusters 1 and 2 the name/number of the subordinates of cluster 0
        for (int i = 0; i < record_nr_workers[rank][0]; i++) {

            MPI_Send(&record[rank][i], 1, MPI_INT, 1, rank, MPI_COMM_WORLD);
            printf("M(0,1)\n");
            fflush(stdout);

            MPI_Send(&record[rank][i], 1, MPI_INT, 2, rank, MPI_COMM_WORLD);
            printf("M(0,2)\n");
            fflush(stdout);
        }


        // receive from cluster 1 the number of subordinates
        MPI_Recv(&record_nr_workers[1][0], 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        record[1] = (int *)malloc(record_nr_workers[1][0] * sizeof(int));

        // receive from cluster 1 the name/number of subordinates
        for (int i = 0; i < record_nr_workers[1][0]; i++) {
            MPI_Recv(&record[1][i], 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        // receive from cluster 2 the number of subordinates
        MPI_Recv(&record_nr_workers[2][0], 1, MPI_INT, 2, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        record[2] = (int *)malloc(record_nr_workers[2][0] * sizeof(int));

        // receive from cluster 2 the name/number of subordinates
        for (int i = 0; i < record_nr_workers[2][0]; i++) {
            MPI_Recv(&record[2][i], 1, MPI_INT, 2, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        print_cluster(rank, record, record_nr_workers);

        share_to_subordinate(rank, record, record_nr_workers);
    
    } else if (rank == 1) {

        // sending the clusters 0 and 2 how many subordinates cluster 1 has
        MPI_Send(&record_nr_workers[rank][0], 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
        printf("M(1,0)\n");
        fflush(stdout);

        MPI_Send(&record_nr_workers[rank][0], 1, MPI_INT, 2, rank, MPI_COMM_WORLD);
        printf("M(1,2)\n");
        fflush(stdout);

        // sending the clusters 0 and 2 the name/number of the subordinates of cluster 1
        for (int i = 0; i < record_nr_workers[rank][0]; i++) {
            MPI_Send(&record[rank][i], 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
            printf("M(1,0)\n");
            fflush(stdout);

            MPI_Send(&record[rank][i], 1, MPI_INT, 2, rank, MPI_COMM_WORLD);
            printf("M(1,2)\n");
            fflush(stdout);
        }


        // receive from cluster 0 the number of subordinates
        MPI_Recv(&record_nr_workers[0][0], 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        record[0] = (int *)malloc(record_nr_workers[0][0] * sizeof(int));

        // receive from cluster 0 the name/number of subordinates
        for (int i = 0; i < record_nr_workers[0][0]; i++) {
            MPI_Recv(&record[0][i], 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        // receive from cluster 2 the number of subordinates
        MPI_Recv(&record_nr_workers[2][0], 1, MPI_INT, 2, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        record[2] = (int *)malloc(record_nr_workers[2][0] * sizeof(int));

        // receive from cluster 2 the name/number of subordinates
        for (int i = 0; i < record_nr_workers[2][0]; i++) {
            MPI_Recv(&record[2][i], 1, MPI_INT, 2, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        print_cluster(rank, record, record_nr_workers);

        share_to_subordinate(rank, record, record_nr_workers);

    } else if (rank == 2) {

        // sending the clusters 0 and 1 how many subordinates cluster 2 has
        MPI_Send(&record_nr_workers[rank][0], 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
        printf("M(2,0)\n");
        fflush(stdout);

        MPI_Send(&record_nr_workers[rank][0], 1, MPI_INT, 1, rank, MPI_COMM_WORLD);
        printf("M(2,1)\n");
        fflush(stdout);

        // sending the clusters 0 and 1 the name/number of the subordinates of cluster 2
        for (int i = 0; i < record_nr_workers[rank][0]; i++) {
            MPI_Send(&record[rank][i], 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
            printf("M(2,0)\n");
            fflush(stdout);

            MPI_Send(&record[rank][i], 1, MPI_INT, 1, rank, MPI_COMM_WORLD);
            printf("M(2,1)\n");
            fflush(stdout);
        }

        // receive from cluster 0 the number of subordinates
        MPI_Recv(&record_nr_workers[0][0], 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        record[0] = (int *)malloc(record_nr_workers[0][0] * sizeof(int));
       
        // receive from cluster 0 the name/number of subordinates
        for (int i = 0; i < record_nr_workers[0][0]; i++) {
            MPI_Recv(&record[0][i], 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        // receive from cluster 1 the number of subordinates
        MPI_Recv(&record_nr_workers[1][0], 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        record[1] = (int *)malloc(record_nr_workers[1][0] * sizeof(int));

        // receive from cluster 1 the name/number of subordinates
        for (int i = 0; i < record_nr_workers[1][0]; i++) {
            MPI_Recv(&record[1][i], 1, MPI_INT, 1, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        }

        print_cluster(rank, record, record_nr_workers);

        share_to_subordinate(rank, record, record_nr_workers);

    } else {
        
        for (int i = 0; i < 3; i++) {
            MPI_Recv(&record_nr_workers[i][0], 1, MPI_INT, master, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            record[i] = (int *)malloc(record_nr_workers[i][0] * sizeof(int));

            for (int j = 0; j < record_nr_workers[i][0]; j++) {
                MPI_Recv(&record[i][j], 1, MPI_INT, master, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            }
        }
        
        print_cluster(rank, record, record_nr_workers);
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int *chunk_arr;
    int chunk_size;
    int rank_subord;

    // performing calculations
    if (rank == 0) {

        int start, end;

        for (int k = 0; k < size; k++) {
            V[k] = k;
        }
        
        // sending data for cluster 1 and 2
        for (int proc = 0; proc < 3; proc++) {
            
            for (int i = 0; i < record_nr_workers[proc][0]; i++) {

                start = (record[proc][i] - 3) * (double)size / (numtasks - 3);
                end = MIN(((record[proc][i] - 3) + 1) * (double)size / (numtasks - 3), size);
                chunk_size = end - start;

                if (proc != 0 ) { 
                    MPI_Send(&chunk_size, 1, MPI_INT, proc, rank, MPI_COMM_WORLD);
                    printf("M(0,%d)\n", proc);
                    fflush(stdout);

                    MPI_Send(V + start, chunk_size, MPI_INT, proc, rank, MPI_COMM_WORLD);
                    printf("M(0,%d)\n", proc);
                    fflush(stdout);
                } else {
                    MPI_Send(&chunk_size, 1, MPI_INT, record[proc][i], rank, MPI_COMM_WORLD);
                    printf("M(0,%d)\n", record[proc][i]);
                    fflush(stdout);

                    MPI_Send(V + start, chunk_size, MPI_INT, record[proc][i], rank, MPI_COMM_WORLD);
                    printf("M(0,%d)\n", record[proc][i]);
                    fflush(stdout);
                }
            }
        }


        // receive data from the subordinated workers
        for (int proc = 0; proc < 3; proc++) {
            
            for (int i = 0; i < record_nr_workers[proc][0]; i++) {
                if (proc == 0) {
                    MPI_Recv(&rank_subord, 1, MPI_INT, record[proc][i], MPI_ANY_TAG, MPI_COMM_WORLD, &status);

                    MPI_Recv(&chunk_size, 1, MPI_INT, record[proc][i], MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                    start = (rank_subord - 3) * (double)size / (numtasks - 3);
                    
                    MPI_Recv(V + start, chunk_size, MPI_INT, record[proc][i], MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                } else {
                    MPI_Recv(&rank_subord, 1, MPI_INT, proc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

                    MPI_Recv(&chunk_size, 1, MPI_INT, proc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                    start = (rank_subord - 3) * (double)size / (numtasks - 3);
                   
                    MPI_Recv(V + start, chunk_size, MPI_INT, proc, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                }
            }
        }
        printf("Rezultat: ");
        for (int k = 0; k < size; k++) { 
            printf("%d ", V[k]);         
        }         
        printf("\n");

    } else if (rank == 1 || rank == 2) {

        // receive from 0 the fragments for each subordinated worker and send it to them
        for (int i = 0; i < record_nr_workers[rank][0]; i++) {
            MPI_Recv(&chunk_size, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            chunk_arr = (int *)malloc(chunk_size * sizeof(int));
            MPI_Recv(chunk_arr, chunk_size, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            MPI_Send(&chunk_size, 1, MPI_INT, record[rank][i], rank, MPI_COMM_WORLD);
            printf("M(%d,%d)\n",rank, record[rank][i]);
            fflush(stdout);

            MPI_Send(chunk_arr, chunk_size, MPI_INT, record[rank][i], rank, MPI_COMM_WORLD);
            printf("M(%d,%d)\n",rank, record[rank][i]);
            fflush(stdout);
        }

        // receive data from the subordinated workers and sent to 0
        for (int i = 0; i < record_nr_workers[rank][0]; i++) {
            MPI_Recv(&rank_subord, 1, MPI_INT, record[rank][i], MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            MPI_Recv(&chunk_size, 1, MPI_INT, record[rank][i], MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            
            chunk_arr = (int *)malloc(chunk_size * sizeof(int));
            MPI_Recv(chunk_arr, chunk_size, MPI_INT, record[rank][i], MPI_ANY_TAG, MPI_COMM_WORLD, &status);

            MPI_Send(&rank_subord, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
            printf("M(%d,0)\n",rank);
            fflush(stdout);

            MPI_Send(&chunk_size, 1, MPI_INT, 0, rank, MPI_COMM_WORLD);
            printf("M(%d,0)\n",rank);
            fflush(stdout);

            MPI_Send(chunk_arr, chunk_size, MPI_INT, 0, rank, MPI_COMM_WORLD);
            printf("M(%d,0)\n",rank);
            fflush(stdout);   
        }

    } else {
        MPI_Recv(&chunk_size, 1, MPI_INT, master, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        chunk_arr = (int *)malloc(chunk_size * sizeof(int));

        MPI_Recv(chunk_arr, chunk_size, MPI_INT, master, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

        for (int i = 0; i < chunk_size; i++) {
            chunk_arr[i] = chunk_arr[i] * 2;
        }
        
        // we proceesed the data so we should send it back
        rank_subord = rank;
        MPI_Send(&rank_subord, 1, MPI_INT, master, rank, MPI_COMM_WORLD);
        printf("M(%d,%d)\n",rank, master);
        fflush(stdout);

        MPI_Send(&chunk_size, 1, MPI_INT, master, rank, MPI_COMM_WORLD);
        printf("M(%d,%d)\n",rank, master);
        fflush(stdout);

        MPI_Send(chunk_arr, chunk_size, MPI_INT, master, rank, MPI_COMM_WORLD);
        printf("M(%d,%d)\n",rank, master);
        fflush(stdout);
    }

    MPI_Finalize();
    return 0;
}
