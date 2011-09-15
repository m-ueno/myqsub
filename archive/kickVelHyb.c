#include <math.h>
#include "nbody.h"

extern float pos[NP][NDIM];
extern float vel[NP][NDIM];
extern float mass;
extern int irank, nrank;
extern int lhp[MAXRANK+1], lnp[MAXRANK];

/* kickVelMPI.c */
void kickVel (double delt) {
	int i,j;
	float rij2;
	float pos_ji_x, pos_ji_y, pos_ji_z;
	double ax,ay,az;
	double mass_denominator;

#pragma omp parallel private( i, ax,ay,az, j, pos_ji_x, pos_ji_y, pos_ji_z, rij2, mass_denominator ) //shared (pos, vel, delt, mass)
	{

#pragma omp for
		for( i=lhp[irank]; i<lhp[irank+1]; i++ ){
			ax=ay=az=0;

			for( j=0; j<NP; j++ ){
				if( i==j ) continue;
				pos_ji_x = pos[j][0]-pos[i][0];
				pos_ji_y = pos[j][1]-pos[i][1];
				pos_ji_z = pos[j][2]-pos[i][2];

				// calc rij**2
				rij2 = pos_ji_x*pos_ji_x + pos_ji_y*pos_ji_y + pos_ji_z*pos_ji_z + EPSSOFT2;

				mass_denominator = GCONST * mass / ( sqrtf(rij2)*rij2 ); //GCONST=1.0, mass=1/NP;

				ax += pos_ji_x * mass_denominator;
				ay += pos_ji_y * mass_denominator;
				az += pos_ji_z * mass_denominator;
			}// end for(j)

			// update vel[i]
			vel[i][0] += ax * delt;
			vel[i][1] += ay * delt;
			vel[i][2] += az * delt;
		}// end for(i)
		return;
	}
}
