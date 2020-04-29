#include "../lib/knn.h"
#include <stdio.h>
#include "mpi.h"
#include <omp.h>
int main(int argc, char **argv){
	int index_i;
	int correct_Number=0,error_Number=0, error_Number_Temp=0,correct_Number_Temp=0;
	letter * p;
	char c;
	int rank, size;
	//MPI_Status status;
	MPI_Datatype letterType;
	MPI_Datatype type[3] = { MPI_CHAR, MPI_INT, MPI_FLOAT };
	MPI_Aint displs_Array[3];
	int blocklen[3] = { 1, 16, 1 };
	MPI_Aint disp[3];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD,&rank);
	MPI_Comm_size(MPI_COMM_WORLD,&size);
	/*int MPI_Type_create_struct(
	  int count,
	  int array_of_blocklengths[],
	  MPI_Aint array_of_displacements[],
	  MPI_Datatype array_of_types[],
	  MPI_Datatype *newtype
	  );*/
	MPI_Address(&letters[0].c, &displs_Array[0]);
	MPI_Address(&letters[0].array[0],&displs_Array[1]);
	MPI_Address(&letters[0].distance,&displs_Array[2]);
	disp[0] = 0;
	disp[1] = displs_Array[1] - displs_Array[0];
	disp[2] = displs_Array[2] - displs_Array[0];
	MPI_Type_create_struct(3, blocklen, disp, type, &letterType);
	MPI_Type_commit(&letterType);

	if (!rank){
		printf("训练样本为%d\n",M);
		Get_from_letters();
		printf("测试样本为%d\n",N);
		Get_from_nletters();
	}
	MPI_Barrier(MPI_COMM_WORLD);
	MPI_Bcast(&(letters[0]), M, letterType, 0, MPI_COMM_WORLD);
	MPI_Scatter(&(nletters[0]), N/size, letterType ,&(nletters_temp[0]), N/size, letterType,0,MPI_COMM_WORLD);
	for(index_i=0; index_i<N/size; index_i++){
		p=&nletters_temp[index_i];
		Distance(p);
		select_Sort();
		c=Knn(K);
		if(nletters_temp[index_i].c==c){
			printf("第%d个字符属于%c类,识别正确!\n",index_i+rank*N/size+1,nletters_temp[index_i].c);
			correct_Number++;
		}
		else{
			printf("第%d个字符属于%c类，识别为%c类,识别错误!\n",index_i+rank*N/size+1, nletters_temp[index_i].c, c);
			error_Number++;
		}
	}
	MPI_Reduce(&correct_Number, &correct_Number_Temp, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);// 所有的进程都要调用，而不是只在目标进程中调用
	MPI_Reduce(&error_Number, &error_Number_Temp, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);// 所有的进程都要调用，而不是只在目标进程中调用

	if (!rank){
		printf("\n正确个数为%d",correct_Number_Temp);
		printf("错误个数为%d",error_Number_Temp);
		printf("正确率为%f",(float)(correct_Number_Temp)/N);
	}
	MPI_Type_free(&letterType);
	MPI_Finalize();
	return 0;
}
