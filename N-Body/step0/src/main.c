#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>
#include <mpi.h>
#include "../lib/N_Body.h"

int main(int argc, char* argv[]){

   // Initialise problem parameters
   double start,end;
   int myid, size;
   if (argc >= 2) 
      sscanf(argv[1], "%i", &N);
   else
      N = DEFAULT_N;

   if (argc >= 3)
      sscanf(argv[2], "%i", &TIME);   
   else
      TIME = DEFAULT_TIME;
   MPI_Init(&argc, &argv);
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   particlenum = N / size;
   if (myid == size-1) {
	   particlenum = N - particlenum*(size-1);
   }
   // Allocate memory for mass, disance, velocity and force arrays
   myposition = (Position *)malloc(particlenum * sizeof(Position));
   mass = (double *) malloc(N * sizeof(double));
   radius = (double *) malloc(N * sizeof(double));
   position = (Position *) malloc(N * sizeof(Position));
   velocity = (Velocity *) malloc(N * sizeof(Velocity));
   force = (Force *) malloc(N * sizeof(Force));
   if (myid == 0) {
	   position2 = (Position *)malloc(N * sizeof(Position));
		read_space();
		read_positions();
	}
   int left;
   left = myid*particlenum;
   start = omp_get_wtime();
   MPI_Bcast(mass, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Bcast(radius, N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
   MPI_Bcast(velocity, sizeof(Velocity)*N, MPI_BYTE, 0, MPI_COMM_WORLD);
   MPI_Bcast(position, sizeof(Position)*N, MPI_BYTE, 0, MPI_COMM_WORLD);
   MPI_Scatter(&(position[left]), sizeof(Position)*particlenum, MPI_BYTE, &(myposition[0]), sizeof(Position)*particlenum, MPI_BYTE, 0, MPI_COMM_WORLD);
   // Run the N-body simulation
   run_simulation();

   end = omp_get_wtime();
   if (myid == 0) {
	   printf("Using time = %lf\n",end -start);
	   check_data();
	   free(position2);
   }
   free(mass);
   free(radius);
   free(position);
   free(velocity);
   free(force);
   

   return 0;                   
}
