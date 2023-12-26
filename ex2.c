#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

#define ARR_SIZE 12

int find_max(int arr[], int size) {
    int i; 
    int max = arr[0]; 
    for (i = 1; i < size; i++) 
        if (arr[i] > max) 
            max = arr[i]; 
    return max; 
}

int main(int argc, char *argv[]){

    srand(time(NULL));
    int block_length, rank, size, local_max, global_max;
    int arr[ARR_SIZE];

    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    block_length = ARR_SIZE / size;
    int data[block_length];

    int arr_of_max[size];

    if (rank == 0) {
        printf("Process %d: Array is initialized: [", rank);fflush(stdout);
        for (int i = 0; i < ARR_SIZE; i++) {
            arr[i] = rand() % 100;
            printf("%d,", arr[i]);fflush(stdout);
        }
        puts("]");fflush(stdout);
    }
    MPI_Scatter(arr, block_length, MPI_INTEGER, data, block_length, MPI_INTEGER, 0, MPI_COMM_WORLD);
    local_max = find_max(data, block_length);
    printf("Process %d local max = %d\n", rank,local_max);fflush(stdout);

    MPI_Reduce(&local_max, &global_max, 1, MPI_INTEGER, MPI_MAX, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        printf("Max = %d found by MPI_Reduce()\n", global_max);fflush(stdout);
    }
    
    MPI_Finalize();
    return 0;
}