#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "nbody.h"
#include <time.h>
#include <sys/time.h>

float pos[NP][NDIM];
float vel[NP][NDIM];
float mass = 1.f / NP;

void driftPos (double delt) {
	for( int i=0;i<NP;i++ )
		for( int dim=0; dim<NDIM; dim++)
			pos[i][dim] += delt*vel[i][dim];
}

void readInit (void) {
	FILE *fp;
	fp = fopen ("init.dat", "r");
	fread ((float *) pos, sizeof(float), NP * NDIM, fp);
	fread ((float *) vel, sizeof(float), NP * NDIM, fp);
	fclose (fp);
}

void maxRad () {
	int ip;
	float maxrad=0.0f;
	for (ip=0; ip<NP; ip++) {
		float rad2 = 
			pos[ip][0] * pos[ip][0] +
			pos[ip][1] * pos[ip][1] +
			pos[ip][2] * pos[ip][2];
		maxrad = maxrad > rad2 ? maxrad : rad2;
	}
	fprintf (stderr, "%f\n", sqrtf(maxrad));
}
int main (int argc, char *argv[]) {
	int iter;
	double t1,t2;
	t1 = omp_get_wtime();
	readInit ();
	
	kickVel(0.5 * DELT);
	for (iter=0; iter<NITER; iter++){
		//maxRad();
		fprintf(stderr, ".");
		driftPos(DELT);
		kickVel(DELT);	
	}
	printf("\nexit loop -- main\n");
	driftPos(0.5 * DELT);

	cmpFin();
	t2=omp_get_wtime();
	printf("\ntime: %g\n", t2-t1);

	return 0;
}
