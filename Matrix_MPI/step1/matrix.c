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
	int i, j, k;
	int tmptag = 0;
	double close =0.0;
	MPI_Status status;
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	if(!rank){
		printf("Calculation of starting matrix multiplication...\n");
		for(index_i = 1; index_i< min(size,ROW+1); index_i++){
			MPI_Send(&(matrix1[index_i-1][0]), COL, MPI_DOUBLE, index_i, tmptag+1, MPI_COMM_WORLD);
			tmptag++;
		}

		for(index_i=0; index_i<ROW; ++index_i){
			MPI_Recv(&(tmp2[0][0]),COL,MPI_DOUBLE, MPI_ANY_SOURCE,MPI_ANY_TAG, MPI_COMM_WORLD,&status);
			for(index_j=0; index_j<COL; ++index_j){
					matrix3[status.MPI_TAG-1][index_j]= tmp2[0][index_j];
			}
			if(tmptag <ROW){
				MPI_Send(&(matrix1[tmptag][0]),COL,MPI_DOUBLE,status.MPI_SOURCE, tmptag+1,MPI_COMM_WORLD);
				tmptag++;
			}
			else{
				MPI_Send(&close, 1, MPI_DOUBLE, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
			}
		}
	}
	else{
		while(1){
			MPI_Recv(&(tmp1[0][0]), COL, MPI_DOUBLE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
			if(status.MPI_TAG !=0){
				index_k =0;
				for(index_i=0; index_i<ROW; index_i++){
					sum =0.0;
					for(index_j=0; index_j<COL; index_j++){
						sum +=tmp1[0][index_j]*matrix2[index_i][index_j];
					}
					tmp2[0][index_k++] = sum;

				}
				MPI_Send(&(tmp2[0][0]), COL, MPI_DOUBLE, 0, status.MPI_TAG, MPI_COMM_WORLD);
			}
			else{
				break;
			}
		}
	}
	if(!rank){
		printf("Matrix multiplication calculation complete...\n\n");
		
	}
}
