#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){

  int size, rank, dest, source, N, C, sum, tag=1;
  MPI_Status Stat;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  //Broadcast N (total number of elements)and C (seed)
  if (rank == 0) {
    N = argv[1];
    C = argv[2];
  };
  MPI_Bcast(&N, 1, MPI_INT, 0, MPI_COMM_WORLD); 
  MPI_Bcast(&C, 1, MPI_INT, 0, MPI_COMM_WORLD); 

  //sum locally  1, 2, 3, 4.... n
  int local_sum = 0, g = N / size;
  for (int i = 0; i < g; i++) {
    int temp = rank * g + 1 + i;
    local_sum += temp;
  }


  //
  int d = log(size) / log(2);
  for (int j = 0; j < d; j++) {
    if(rank & pow(2, j) != 0) {
      dest = rank ^ pow(2, j);
      printf("sending from rank %d/%d to %d.\n", rank, size, dest);
      MPI_Send(&local_sum, 1, MPI_INT, dest, tag, MPI_COMM_WORLD);
      break;
    } else {
      source = rank ^ pow(2, j);
      MPI_Recv(&sum, 1, MPI_INT, source, tag, MPI_COMM_WORLD, &Stat);
      local_sum += sum;
      printf("rank %d/%d receiving from %d.\n", rank, size, Stat.MPI_SOURCE);
    }
  }

  if (rank == 0) 
    printf ("\nSum is %d\n", local_sum)

  MPI_Finalize();
  return 0;
}