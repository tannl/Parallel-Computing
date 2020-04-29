#include <stdio.h>
#include "mpi.h"
#include "matrix.h"
int main(int argc, char *argv[]){
	int rank, size;
	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	matrix1 = MATRIX_MALLOC(double,ROW,COL);
	matrix2 = MATRIX_MALLOC(double,ROW,COL);
	matrix3 = MATRIX_MALLOC(double,ROW,COL);
	matrix4 = MATRIX_MALLOC(double,ROW,COL);
	if(!rank)
		read_Data();
	matrix_Compute();
	if(!rank)
		check_Data();
	MPI_Finalize();
	return 0;
}

