#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <sys/resource.h>
#include "nbody.h"

float mass;
float pos[NP][NDIM];
float vel[NP][NDIM];

// lnp: number of particle which Proc i control
// list of number of particles
// lhp: for Proc i, store head index of array ``pos''
// list of head particle-index
int lnp[MAXRANK], lhp[MAXRANK+1];

#define DEBUG

#define TAGX 0
#define TAGV 1
#define TAGG 2

int irank, nrank;

void readInit (void) {
    FILE *fp;
    fp = fopen ("init.dat", "r");
    fread ((float *) pos, sizeof(float), NP * NDIM, fp);
    fread ((float *) vel, sizeof(float), NP * NDIM, fp);
    fclose (fp);
}

void defRange (void) {
    // init lnp[], lhp[].
    // 割り切れない場合.
    // rank数が小さいやつには、もう一ブロック割り当てる.

    int i, np, remainder;
    // ex. NP=102; nrank=4
    np = NP/nrank; //           np = 25 = 102 / 4
    remainder = NP - np * nrank; // 2 = 25 * 4

    lhp[0]=0;
    for( i=0;i<nrank;i++ ){ // i:rank
        if( i<remainder ){
            lnp[i] = np+1; 
        }else{
            lnp[i] = np;
        }
        lhp[i+1] = lhp[i]+lnp[i];
    }
}
void scatterInit (void) {
    /* Send data from rank0 to the rest processes. */
    int jrank;
    printf("%d -- scatteInit\n",irank);
    if( irank==0 ){
        for( jrank=1; jrank<nrank; jrank++ ){
//          MPI_Send( pos+lhp[jrank], lnp[jrank]*NDIM, MPI_FLOAT, jrank, 0, MPI_COMM_WORLD);
            MPI_Send( vel+lhp[jrank], lnp[jrank]*NDIM, MPI_FLOAT, jrank, 1, MPI_COMM_WORLD);
            printf("%d send to %d\n",irank,jrank);
        }
    }else{
//      MPI_Recv( pos+lhp[irank], lnp[irank]*NDIM, MPI_FLOAT, 0, 0, MPI_COMM_WORLD , MPI_STATUS_IGNORE); // (0,0) = (source,tag)
        MPI_Recv( vel+lhp[irank], lnp[irank]*NDIM, MPI_FLOAT, 0, 1, MPI_COMM_WORLD , MPI_STATUS_IGNORE); 
        printf("%d received\n",irank);
    }

    MPI_Bcast(pos, NP*NDIM, MPI_FLOAT, 0, MPI_COMM_WORLD);
}
void driftPos (double delt) {
    /* 自分の担当粒子を動かし、
     * 新しくなった粒子情報を通信士合う。 */
    int i,jrank;
    for( i = lhp[irank]; i < lhp[irank+1]; i++ ){
        pos[i][0] += delt*vel[i][0];
        pos[i][1] += delt*vel[i][1];
        pos[i][2] += delt*vel[i][2];
    }
    for( jrank=0; jrank<nrank; jrank++ ){
        if(irank!=jrank){
            MPI_Send( pos+lhp[irank], lnp[irank]*NDIM, MPI_FLOAT, jrank, 0, MPI_COMM_WORLD);
            MPI_Recv( pos+lhp[jrank], lnp[jrank]*NDIM, MPI_FLOAT, jrank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//          MPI_Sendrecv(pos+lhp[irank], lnp[irank]*NDIM, MPI_FLOAT, jrank, TAGX,
//                  pos+lhp[jrank], lnp[jrank]*NDIM, MPI_FLOAT, jrank, TAGX, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //??
        }
    }
}

void gatherData (float *data) {
    // Finally gather vel.(pos is already sendrecv-ed)
    int jrank;
    if( irank==0 ){
        for( jrank=1; jrank<nrank; jrank++ ){
            MPI_Recv( vel+lhp[jrank], lnp[jrank]*NDIM, MPI_FLOAT, jrank, TAGG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("%d: received from %d --gatherData\n",irank,jrank);
        }
    } else{ // irank>0
        MPI_Send( vel+lhp[irank], lnp[irank]*NDIM, MPI_FLOAT, 0, TAGG, MPI_COMM_WORLD); // (0,0) = (dest,tag)
    }
}

double getrusage_sec(){
    struct rusage t;
    struct timeval tv;
    getrusage(RUSAGE_SELF, &t);
    tv = t.ru_utime;
    return tv.tv_sec + (double)tv.tv_usec*1e-6;
}
int main (int argc, char *argv[]) {
    int iter;
    static double t1,t2;
    if(irank==0) t1 = MPI_Wtime();

    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &nrank);
    MPI_Comm_rank (MPI_COMM_WORLD, &irank);
    mass = 1.f / NP;

    printf("dbg: hello! -- main:%d\n",irank);

    defRange();

    if (irank==0) readInit();
    scatterInit();

    // -- calc
    kickVel(0.5 * DELT);

    for (iter=0; iter<NITER; iter++){
        driftPos(DELT);
        kickVel(DELT);  
    }
    driftPos(0.5 * DELT);
    // ----

    //velocity only
    gatherData(NULL);   //gatherData(float* data)

    if (irank==0) cmpFin();

    printf("%d: MPI_Finalize -- main\n",irank);

    MPI_Finalize ();
    if(irank==0){
        //  t2=getrusage_sec();
        t2 = MPI_Wtime();
        printf("\ntime: %.5g\n",t2-t1);
    }
    return 0;
}
