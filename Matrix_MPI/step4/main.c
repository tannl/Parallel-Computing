#include <stdio.h>
#include "matrix.h"
#include "mpi.h"
int main(int argc, char *argv[]){
    int rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
    matrix1 = MATRIX_MALLOC(double,ROW,COL);
	matrix2 = MATRIX_MALLOC(double,ROW,COL);
	matrix3 = MATRIX_MALLOC(double,ROW,COL);
	matrix4 = MATRIX_MALLOC(double,ROW,COL);
    tmp1 = MATRIX_MALLOC(double,10,COL);
    tmp2 = MATRIX_MALLOC(double,10,COL);
    if(!rank){
        read_Data();
        matrix_Transposition();
    }
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&(matrix2[0][0]), ROW*COL, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    matrix_Compute();
    MPI_Barrier(MPI_COMM_WORLD);
    if(!rank)
        check_Data();
    MPI_Finalize();
	return 0;
}
	
