#include <stdio.h>
#include <mpi.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <unistd.h>
#include <string>
#include <unistd.h>

#define MATSIZE 999
#define PROC_NUM 3

using namespace std;

int A[3000][3000];
int B[3000][3000];
int C[3000][3000];
int c_arr[MATSIZE*MATSIZE];
int C_fin[MATSIZE][MATSIZE];

void loadC() {
    std::fstream cfile("csmall.txt", std::ios_base::in);
    int c_temp;
    int c_rows = 0;
    int c_counter = 0;
    while (cfile >> c_temp) {
        C[c_rows][c_counter] = c_temp;
        c_counter++;
        if (c_counter == MATSIZE) {
            c_rows++;
            c_counter = 0;
        }
    }
    cfile.close();
}

void printA() {
    for (int i = 0; i < MATSIZE; i++) {
        for (int j = 0; j < MATSIZE; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
}

void printB() {
    for (int i = 0; i < MATSIZE; i++) {
        for (int j = 0; j < MATSIZE; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }
}

void printC() {
    for (int i = 0; i < MATSIZE; i++) {
        for (int j = 0; j < MATSIZE; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n\n\n\n");
    }
}

int getxyz(int x, int y, int z) {
    return MATSIZE*MATSIZE*x+MATSIZE*y+z;
}

double start, stop;
int division;
int main(int argc, char *argv[]) {
    int numer_procesu;
    int ile_procesow;
    int ranks[] = {0};
    if (MATSIZE % PROC_NUM == 0) {
        division = MATSIZE / PROC_NUM;
    } else {
        division = 1;
    }
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&numer_procesu);
    MPI_Comm_size(MPI_COMM_WORLD,&ile_procesow);
    int result = 0;
    int result_fin = 0;
        if (numer_procesu == 0) {
            std::fstream afile("asmall.txt", std::ios_base::in);
            std::fstream bfile("bsmall.txt", std::ios_base::in);
            int a;
            int counter = 0;
            int rows = 0;
            while (afile >> a and rows < MATSIZE) {
                A[rows][counter] = a;
                counter++;
                if (counter == MATSIZE) {
                    rows++;
                    counter = 0;
                }
            }
            counter = 0;
            rows = 0;
            while (bfile >> a and rows < MATSIZE) {
                B[rows][counter] = a;
                counter++;
                if (counter == MATSIZE) {
                    rows++;
                    counter = 0;
                }
            }
            afile.close();
            bfile.close();
	    start = MPI_Wtime();
            int counter_row = 0;
            int counter_col = 0;
            int starting_point = 1;
            int initial_starting_point = 1;
            int counter_row_b = 0;
            int counter_col_b = 0;
            int starting_point_b = 1;
            int initial_starting_point_b = 1;
            int ranks_to_send[(PROC_NUM*PROC_NUM)+1];
            int ranks_to_send_b[(PROC_NUM*PROC_NUM)+1];
            int counter_row_num = 0;
            int counter_row_num_b = 0;
            int array_counter = 1;
            int array_counter_b = 1;
            for (int z = 1; z <= PROC_NUM*PROC_NUM*PROC_NUM; z++) {
                ranks_to_send[array_counter+((z-1)%PROC_NUM)*PROC_NUM] = z;
                if (z % PROC_NUM == 0) {
                    counter_row_num++;
                    z = PROC_NUM*PROC_NUM*counter_row_num + PROC_NUM * counter_row_num;
                    array_counter++;
                }
            }
            for (int z = 1; z <= PROC_NUM*PROC_NUM*PROC_NUM; z += PROC_NUM) {
                ranks_to_send_b[array_counter_b] = z;
                array_counter_b++;
                if ((z-1)/(PROC_NUM*PROC_NUM) != (z-1+PROC_NUM)/(PROC_NUM*PROC_NUM)) {
                    z++;
                }
            }
            //sendA
            for (int i = 1; i <= PROC_NUM*PROC_NUM; i++) {
                int temp_starting_point = starting_point;
                int buffor[division*division];
                for (int j = 1; j <= division*division; j++) {
                    buffor[j-1] = A[(temp_starting_point-1)/MATSIZE][(temp_starting_point-1)%MATSIZE];
                    if (j % division == 0) {
                        temp_starting_point += MATSIZE - division;
                    }
                    temp_starting_point++;
                }
                int status_to_send = 1;
                MPI_Send(buffor, division*division, MPI_INT, ranks_to_send[i], 0, MPI_COMM_WORLD);
                MPI_Send(&status_to_send, 1, MPI_INT, ranks_to_send[i], 1, MPI_COMM_WORLD);
                if (i % PROC_NUM == 0) {
                    starting_point = i * division * division + 1;
                } else {
                    starting_point += division;
                }
            }
            //SendB
            for (int i = 1; i <= PROC_NUM*PROC_NUM; i++) {
                int temp_starting_point_b = starting_point_b;
                int buffor[division*division];
                for (int j = 1; j <= division*division; j++) {
                    buffor[j-1] = B[(temp_starting_point_b-1)/MATSIZE][(temp_starting_point_b-1)%MATSIZE];
                    if (j % division == 0) {
                        temp_starting_point_b += MATSIZE - division;
                    }
                    temp_starting_point_b++;
                }
                int status_to_send = 1;
                MPI_Send(buffor, division*division, MPI_INT, ranks_to_send_b[i], 2, MPI_COMM_WORLD);
                MPI_Send(&status_to_send, 1, MPI_INT, ranks_to_send_b[i], 3, MPI_COMM_WORLD);
                if (i % PROC_NUM == 0) {
                    starting_point_b = i * division * division + 1;
                } else {
                    starting_point_b += division;
                }
            }
        } else {
            int buffora[division*division];
            int bufforb[division*division];
            MPI_Status status;
            int type;
            int type_b;
            //Recieve A
            MPI_Recv(buffora, division*division, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            MPI_Recv(&type, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (type == 1) {
                int offset = (numer_procesu-1)/(PROC_NUM*PROC_NUM);
                int leftoffset = ((PROC_NUM-1)%PROC_NUM)+1;
                for (int z = numer_procesu-(PROC_NUM*offset); z < numer_procesu-(PROC_NUM*offset)+(PROC_NUM*PROC_NUM); z += PROC_NUM) {
                    if (z != numer_procesu) {
                        int temp_type = 0;
                        MPI_Send(&temp_type, 1, MPI_INT, z, 1, MPI_COMM_WORLD);
                        MPI_Send(buffora, division*division, MPI_INT, z, 0, MPI_COMM_WORLD);
                    }
                }
            }
            //Recieve B
            MPI_Recv(bufforb, division*division, MPI_INT, MPI_ANY_SOURCE, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&type_b, 1, MPI_INT, MPI_ANY_SOURCE, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            if (type_b == 1) {
                int offset = (numer_procesu-1)/(PROC_NUM*PROC_NUM);
                int leftoffset = ((PROC_NUM-1)%PROC_NUM)+1;
                int przesuniecie = (numer_procesu-1)/(PROC_NUM*PROC_NUM);
                for (int i = numer_procesu-przesuniecie; i < numer_procesu-przesuniecie+PROC_NUM; i++){
                    if (i != numer_procesu) {
                        int temp_type = 0;
                        MPI_Send(&temp_type, 1, MPI_INT, i, 3, MPI_COMM_WORLD);
                        MPI_Send(bufforb, division*division, MPI_INT, i, 2, MPI_COMM_WORLD);
                    }
                }
            }
            int shift_right = ((numer_procesu-1)%PROC_NUM*PROC_NUM)%PROC_NUM;
            int shift_bottom = ((numer_procesu-1)%PROC_NUM*PROC_NUM)/PROC_NUM;
            int output[MATSIZE*MATSIZE];
            for (int i = 0; i < division; i++) {
                for (int j = 0; j < division; j++) {
                    int sum = 0;
                    for (int k = 0; k < division; k++) {
                        sum += buffora[i*division+k] * bufforb[k*division+j];
                    }
                    C[i+((((numer_procesu-1)%(PROC_NUM*PROC_NUM))%PROC_NUM)*division)][j+((((numer_procesu-1)%(PROC_NUM*PROC_NUM))/PROC_NUM)*division)] = sum;
                }
            }
        }
        for (int i = 0; i < MATSIZE; i++) {
            MPI_Reduce(C[i], C_fin[i], MATSIZE, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
        }

        if (numer_procesu == 0) {
            stop = MPI_Wtime();
        }
    MPI_Finalize();

    return 0;
}
