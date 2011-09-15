#include <math.h>
#include "nbody.h"

extern float pos[NP][NDIM];
extern float vel[NP][NDIM];
extern float mass;

void kickVel (double delt) {
	int i,j;
	float rij2;
	float pos_ji_x, pos_ji_y, pos_ji_z;
	double a[3];
	double mass_denominator;

	//	printf("start -- kickVel\n");
	for( i=0; i<NP; i++ ){ // new!
		a[0]=a[1]=a[2]=0;

		for( j=0; j<NP; j++ ){
			if(i==j) continue;
			pos_ji_x = pos[j][0]-pos[i][0];
			pos_ji_y = pos[j][1]-pos[i][1];
			pos_ji_z = pos[j][2]-pos[i][2];

			// calc rij**2
			rij2 = pos_ji_x*pos_ji_x + pos_ji_y*pos_ji_y + pos_ji_z*pos_ji_z + EPSSOFT2;

			mass_denominator = GCONST * mass / ( sqrtf(rij2)*rij2 );

			a[0] += pos_ji_x * mass_denominator;
//			a[j][0] -= pos_ji_x * mass_denominator;
			a[1] += pos_ji_y * mass_denominator;
//			a[j][1] -= pos_ji_y * mass_denominator;
			a[2] += pos_ji_z * mass_denominator;
//			a[j][2] -= pos_ji_z * mass_denominator;

//			vel[j][0] += a[0] * delt;
//			vel[j][1] += a[1] * delt;
//			vel[j][2] += a[2] * delt;

		} // end for(j)

		// update v[i]
		vel[i][0] += a[0] * delt;
		vel[i][1] += a[1] * delt;
		vel[i][2] += a[2] * delt;
	} // end for(i)
}
