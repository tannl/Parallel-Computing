#ifndef _N_BODY_H
#define _N_BODY_H

#define DEFAULT_N 10000   // Number of particles
#define DEFAULT_TIME 1000 // Number if iterations
#define G 6.67300e-11     // Gravitational constant, m3 kg-1 s-2
#define XBOUND 1.0e6      // Width of space
#define YBOUND 1.0e6      // Height of space
#define ZBOUND 1.0e6      // Depth of space
#define BOUND 1.0e6      //  BOUND of space
#define RBOUND 10         // Upper bound on radius
#define DELTAT 0.01       // Time increament
#define THETA 1.0         // Opening angle, for approximation in BH

#define MASS_OF_JUPITER 1.899e27   //  Sample masses for testing
#define MASS_OF_EARTH 5.974e24     //  and comparing the effect 
#define MASS_OF_MOON 7.348e22      //  of using different values 
#define MASS_OF_UNKNOWN 1.899e12   //  for m.

/* Positional values of particle in space */
typedef struct {

   double px, py, pz;

} Position;

/* Components of velocity of particle */
typedef struct {

   double vx, vy, vz;

} Velocity;

/* Components of force experienced by particle */
typedef struct {

   double fx, fy, fz;

} Force;

Position* myposition;
Position* position;   // Current positions for all particles
Position* position2;   //positions for all particles
Velocity* ivelocity;  // Initial velocity for all particles
Velocity* velocity;   // Velocity of particles in current processor
double* mass;         // Mass of each particle
double* radius;       // Radius of each particle
Force* force;         // Force experienced by all particles
int N;               // User specified particle count
int TIME;            // User specified iterations
int particlenum;     // 每个进程的计算数量

/*
 * Generates a random number in the range 0 to 1
 */
double generate_rand();

/*
 * Generates a random number in the range -1 to 1
 */
double generate_rand_ex();

/*
 * Function to initalize the bodies in space. The particles
 * are given a random locaton (px, py, pz), velocity (vx, vy, vz)
 * and random mass. 
 */
void initialize_space();

/*
 * Checks if two particles in space intersect/collide
 *
 */
int check_collision(int index1, int index2);

/*
 * Computes the distance between two particles in space
 */
double compute_distance(Position a, Position b);


/*
 * Since the initial positions and radii were generated randomly
 * the system would have inherited a set of particles that were
 * already in collision. This method reinitializes the radius of
 * the particles such that the particles are not in collision with
 * each other.
 */
void reinitialize_radius();


/*
 * Computes the forces experienced by each of the particles 
 * as a result of the gravitational attraction with every other
 * particle in space
 */
void compute_force();

/*
 * Computes the new velocities as a result of the 
 * forces experienced
 */
void compute_velocity();

/*
 * Computes the new positions of the particles in space
 */
void compute_positions();

/*
 * Prints the mass of each particle. Used for debugging.
 */
void print_mass();

/*
 * Prints the velocity of each particle. Used for debugging.
 */
void print_velocity();

/*
 * Prints the initial velocity of each particle. Used for
 * debugging.
 */
void print_ivelocity();

/*
 * Prints the position of each particle. Used for debugging.
 */
void print_position();

/*
 * Prints the current values of the bodies in space
 * to the console
 */
void print_space();

/*
 * Runs the N-Body simulation
 */
void run_simulation();


void read_positions();

/*
 * reads data
 *
 */
void read_space();

void check_data();
#endif