#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include <omp.h>
#include <mpi.h>
#include "../lib/N_Body.h"

/*
 * Generates a random number in the range 0 to 1
 */
double generate_rand(){
   return rand()/((double)RAND_MAX + 1);
}


/*
 * Generates a random number in the range -1 to 1
 */
double generate_rand_ex(){
   return 2 * generate_rand() - 1;
}

/*
 * Function to initalize the bodies in space. The particles
 * are given a random locaton (px, py, pz), velocity (vx, vy, vz)
 * and random mass. 
 */
void initialize_space() {   
   
   int i;

   // Inner bounds to prevent generating a particle whose
   // surface lies outside the boundaries of space
   double ixbound = XBOUND - RBOUND;
   double iybound = YBOUND - RBOUND;
   double izbound = ZBOUND - RBOUND;


   for (i = 0; i < N; i++) {
      mass[i] = MASS_OF_UNKNOWN * generate_rand();
      radius[i] = RBOUND * generate_rand();
      position[i].px = generate_rand() * ixbound;
      position[i].py = generate_rand() * iybound;
      position[i].pz = generate_rand() * izbound;
      velocity[i].vx = generate_rand_ex();
      velocity[i].vy = generate_rand_ex();
      velocity[i].vz = generate_rand_ex(); 
   }
}


/*
 * Checks if two particles in space intersect/collide
 *
 */
int check_collision(int index1, int index2) {

   if (pow((position[index1].px - position[index2].px), 2.0) + 
       pow((position[index1].py - position[index2].py), 2.0) +
       pow((position[index1].pz - position[index2].py), 2.0) <
       pow((radius[index1] + radius[index2]), 2.0)) {
       
       // Collision detected
       return 1;

   }   
   return 0;
}


/*
 * Computes the distance between two particles in space
 */
double compute_distance(Position a, Position b){
    return sqrt(pow((a.px - b.px), 2.0) +
               pow((a.py - b.py), 2.0) +
               pow((a.pz - b.pz), 2.0));
}


/*
 * Since the initial positions and radii were generated randomly
 * the system would have inherited a set of particles that were
 * already in collision. This method reinitializes the radius of
 * the particles such that the particles are not in collision with
 * each other.
 */
void reinitialize_radius() {

   int i, j;
   
   for (i = 0; i < N; i++) {

      for (j = i + 1; j < N; j++) {

         if (check_collision(i, j)) {
            double d = compute_distance(position[i], position[j]);
            radius[i] = radius[j] = d/2.0;
         }
      }
   }
}


/*
 * Computes the forces experienced by each of the particles 
 * as a result of the gravitational attraction with every other
 * particle in space
 */
void compute_force(){

   int i, j, myid, size;   
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   int left;

   left = myid*particlenum;
#pragma omp parallel for  num_threads(20),private(j)
   for (i = left; i < left+particlenum; i++) {

      force[i].fx = 0.0;
      force[i].fy = 0.0;
      force[i].fz = 0.0;
      for (j = 0; j < N; j++){

         if (j == i) 
            continue; // avoid computation for 
                     // same bodies

         double d = compute_distance(myposition[i-left], position[j]);

         if(d >= BOUND/2) 
            continue; //The distance is so far that the acting force is ignored
         if(d < sqrt(pow((radius[i] + radius[j]), 2.0))){
            velocity[i].vx *= -1;
            velocity[j].vx *= -1;
            continue; //Collision
                     //The conservation of momentum is not taken into account here.
         } 
           
         // Compute grativational force according to Newtonian's law
         double f = (G * (mass[i] * mass[j]) / 
                         (pow(d, 2.0)));

         // Resolve forces in each direction
         force[i].fx += f * ((position[j].px - myposition[i-left].px) / d);
         force[i].fy += f * ((position[j].py - myposition[i-left].py) / d);
         force[i].fz += f * ((position[j].pz - myposition[i-left].pz) / d);
      }
   }
}


/*
 * Computes the new velocities as a result of the 
 * forces experienced
 */
void compute_velocity(){
   int i;
   int myid, size;
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   int left;

   left = myid*particlenum;
   for (i = left; i < left+particlenum; i++) {
      velocity[i].vx += (force[i].fx / mass[i]) * DELTAT;
      velocity[i].vy += (force[i].fy / mass[i]) * DELTAT;
      velocity[i].vz += (force[i].fz / mass[i]) * DELTAT;
   }
}


/*
 * Computes the new positions of the particles in space
 */
void compute_positions(){
   int i;
   int myid, size;
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   int left;
left = myid*particlenum;
   for (i = left; i < left+particlenum; i++) {
      myposition[i-left].px += velocity[i].vx * DELTAT;
      myposition[i-left].py += velocity[i].vy * DELTAT;
      myposition[i-left].pz += velocity[i].vz * DELTAT;

      // Check if particles attempt to cross boundary      
      if ((myposition[i - left].px + radius[i]) >= XBOUND ||
          (myposition[i - left].px - radius[i]) <= 0)
         velocity[i].vx *= -1;
      else if ((myposition[i - left].py + radius[i] >= YBOUND) ||
               (myposition[i - left].py - radius[i]) <= 0)
         velocity[i].vy *= -1;
      else if ((myposition[i - left].pz + radius[i]) >= ZBOUND ||
               (myposition[i - left].pz - radius[i]) <= 0)
         velocity[i].vz *= -1;      
   }
}

/*
 * Prints the mass of each particle. Used for debugging.
 */
void print_mass(){
   int i;
   for (i = 0; i < N; i++)
      printf("mass=%.2f\n",mass[i]);
   printf("\n");
}

/*
 * Prints the velocity of each particle. Used for debugging.
 */
void print_velocity(){
   int i;
   for (i = 0; i < N; i++)
      printf("vx=%.2f, vy=%.2f, vz=%.2f\n",velocity[i].vx, velocity[i].vy, velocity[i].vz);
   printf("\n");
}


/*
 * Prints the initial velocity of each particle. Used for
 * debugging.
 */
void print_ivelocity(){
   int i;
   for (i = 0; i < N; i++)
      printf("vx=%.2f, vy=%.2f, vz=%.2f\n", ivelocity[i].vx, ivelocity[i].vy, ivelocity[i].vz);
   printf("\n");
}

/*
 * Prints the position of each particle. Used for debugging.
 */
void print_position(){
   int i;
   for (i = 0; i < N; i++)
      printf("px=%.2f, py=%.2f, pz=%.2f\n",position[i].px, position[i].py, position[i].pz);
   printf("\n");
}

/*
 * Prints the current values of the bodies in space
 * to the console
 */
void print_space() {

   int i;

   printf("\n\n Space with %d bodies \n", N);
   for (i = 0; i < N; i++) {
      printf("bodies%d: mass = %.2f, px=%.2f, py=%.2f, pz=%.2f, vx=%.4f, vy=%.4f, vz=%.4f\n", 
             i, mass[i], position[i].px, position[i].py, position[i].pz, velocity[i].vx, 
             velocity[i].vy, velocity[i].vz);
   }
}

void read_positions() {

   FILE* file;

   file = fopen("../data/position_final.dat", "r");

   if (file == NULL) {
       fprintf(stderr,"Cannot open output file\n");
       exit (0);
   }

   int i;
   for (i = 0; i < N; i++) {
       fscanf(file, "%lf %lf %lf\n", &(position2[i].px), &(position2[i].py), &(position2[i].pz));
   }

   fclose(file);
}

void read_space(){
   int i;
   FILE* file;
   file = fopen("../data/space.dat", "r");

   if (file == NULL) {
       fprintf(stderr,"Cannot open output file\n");
       exit (0);
   }
   for (i = 0; i < N; i++) {
      fscanf(file, "%lf %lf %lf %lf %lf %lf %lf %lf\n",
            &(mass[i]),&(radius[i]),&(position[i].px), &(position[i].py),
            &(position[i].pz), &(velocity[i].vx), &(velocity[i].vy), &(velocity[i].vz));
      
   }
}

void check_data(){
   int i;
   int flag = 0;
   for(i = 0; i < N; i++){
      if((fabs(position[i].px - position2[i].px) >= 1e-6) ||
         (fabs(position[i].py - position2[i].py) >= 1e-6) ||
         (fabs(position[i].pz - position2[i].pz) >= 1e-6)){
            flag = 1;
            printf("Data is error!\n");
            break;
         }

   }
   if(!flag){
      printf("The data is correct!\n");
   }
}
/*
 * Runs the N-Body simulation
 */
void run_simulation(){

   int i;
   int myid, size;
   MPI_Comm_rank(MPI_COMM_WORLD, &myid);
   MPI_Comm_size(MPI_COMM_WORLD, &size);
   int left;
   left = myid*particlenum;
   if (myid == 0) {
	   printf("\nRunning simulation for %d bodies with %d iterations, and DELTAT = %f..\n\n", N, TIME, DELTAT);
   }
   for (i = 0; i < TIME; i++) {
      compute_force();
      compute_velocity();
      compute_positions();
MPI_Gather(&(myposition[0]), sizeof(Position)*particlenum, MPI_BYTE, &(position[left]), sizeof(Position)*particlenum, MPI_BYTE, 0, MPI_COMM_WORLD);
MPI_Bcast(position,sizeof(Position)*N,MPI_BYTE,0, MPI_COMM_WORLD);
if(myid==0){

printf("%d time is over\n", i);
}
}
}
