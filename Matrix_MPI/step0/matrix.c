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

void check_Data(){
	int flag = 0;
	int index_i, index_j, index_k;
	printf("Check...\n");
	for(index_i=0; index_i<ROW; index_i++){
		for(index_j= 0; index_j< COL; index_j++){
			if(matrix4[index_i][index_j]- matrix3[index_i][index_j] >= 1e-6){
				printf("error!\n");
				flag = 1;
				break;
			}
		}
	}
	if(!flag){
		printf("correct!\n");
	}
}

void matrix_Compute(){
	int index_i, index_j, index_k;
	double sum=0.0;
	printf("Calculation of starting matrix multiplication...\n");
	for(index_i=0; index_i<ROW; index_i++){
		for(index_j= 0; index_j< COL; index_j++){
			for(index_k=0; index_k< LEN; index_k++){
				sum += matrix1[index_i][index_k] * matrix2[index_k][index_j];
			}
			matrix3[index_i][index_j]= sum;
			sum = 0.0;
		}
	}
	printf("Matrix multiplication calculation complete...\n\n");
}
