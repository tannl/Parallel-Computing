#include "matrix.h"
#include "mpi.h"
void read_Data(){
    FILE *fp1,*fp2;
	int index_i, index_j, index_k;
	double tmp;
    printf("Start reading data...\n");
	fp1 = fopen("../data.txt", "r");
	for(index_i =0; index_i< LEN; index_i++){
		for(index_j =0; index_j< LEN; index_j++){
			fscanf(fp1, "%lf ",&tmp);
			matrix1[index_i][index_j]= matrix2[index_i][index_j]= tmp;
		}
	}
	fclose(fp1);

    fp2 = fopen("../result.txt", "r");
	for(index_i =0; index_i< LEN; index_i++){
		for(index_j =0; index_j<LEN; index_j++){
			fscanf(fp2, "%lf ",&matrix4[index_i][index_j]);
		}
	}
	fclose(fp2);
	printf("Data loading complete...\n\n");

}

int check_Data(){
	int flag = 0;
	int index_i, index_j, index_k;
	printf("Check...\n");
	for(index_i=0; index_i<ROW; index_i++){
		for(index_j= 0; index_j< COL; index_j++){
			if(matrix4[index_i][index_j]- matrix3[index_i][index_j] >= 1e-6){
				printf("error!\n");
				printf("matrix4[index_i][index_j] = %lf\nmatrix3[index_i][index_j] = %lf\nindex_i = %d\nindex_j = %d\n",matrix4[index_i][index_j], matrix3[index_i][index_j], index_i, index_j);
				flag = 1;
				return 1;
			}
		}
	}
	if(!flag){
		printf("correct!\n");
	}
	return 0;
}

void matrix_Transposition(){
	int index_i, index_j;
	double tmp = 0.0;
	for(index_i=0; index_i<ROW; index_i++){
		for(index_j=index_i; index_j<COL; index_j++){
			tmp = matrix2[index_i][index_j];
			matrix2[index_i][index_j] = matrix2[index_j][index_i];
			matrix2[index_j][index_i] = tmp;
		}
	}
}
void matrix_Compute(){
	int index_i, index_j, index_k;
	double sum=0.0;
	int rank, size;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(!rank){
		printf("Calculation of starting matrix multiplication...\n");
	}
	MPI_Scatter(&(matrix1[0][0]),LEN*200,MPI_DOUBLE,&(tmp1[0][0]),LEN*200, MPI_DOUBLE,0,MPI_COMM_WORLD);
	for(index_i=0; index_i<200; index_i++){
		for(index_j= 0; index_j< COL; index_j++){
			for(index_k=0; index_k< LEN; index_k++){
				sum = sum+ tmp1[index_i][index_k] * matrix2[index_j][index_k];				
			}
			tmp2[index_i][index_j]= sum;
			sum = 0.0;
		}
	}
	MPI_Gather(&(tmp2[0][0]),LEN*200,MPI_DOUBLE,&(matrix3[0][0]),LEN*200,MPI_DOUBLE,0,MPI_COMM_WORLD);
	if(!rank){
		printf("Matrix multiplication calculation complete...\n\n");	
	}
}


// MPI_Scatter(sendbuf,sendcount,sendtype,recvbuf,recvcount,recvtype,root,comm);
// MPI_Gather(sendBuf,n,MPI_INT,recvBuf,recvCount,MPI_INT,root,MPI_COMM_WORLD);