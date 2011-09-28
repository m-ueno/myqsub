/* OpenMP: MPI_like */
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NX 193
#define NY 193

int main(int argc, char *argv[]){
    double u[NY+1][NX+1], un[NY+1][NX+1];
    double h = 1.0/NX;
    double dt = 0.1*h*h;
    double dth2 = dt/h/h;
    FILE *udata;
    int i,j,k;
    int irank, nrank;

    double t1,t2;
    t1 = omp_get_wtime();

#pragma omp parallel private (k)
    {
        nrank = omp_get_num_threads(); /* 4 */
        irank = omp_get_thread_num();  /* [0,3] */
        int nx = (NX-1)/nrank;         /* 48 = 192/4 */
        int ny = (NY-1)/nrank;         /* 48 */
        int north=0,south=0;
        if(irank==0) south=-1;   /* 南端-1 */
        if(irank==nrank-1) north=1; /* 北端+1 */


        for(j=irank*ny+1+south; j<(irank+1)*ny+north; j++){
            for(i=0; i<NX+1; i++){
                if (i==0)
                    u[j][i] = 0.5;
                else if (j==0)
                    u[j][i] = 1.0;
                else
                    u[j][i] = 0.0;
            }
        }

#pragma omp_barrier

        for(k=0;k<40000;k++){       // 40000 times loop
#pragma omp_barrier
            for(j=irank*ny+1; j<(irank+1)*ny+1; j++) /* 手動で分割 */
                for(i=1;i<NX;i++)
                    un[j][i] = u[j][i] + ( -4*u[j][i] + u[j][i+1] + u[j][i-1] + u[j+1][i] + u[j-1][i] )*dth2;
            for(j=irank*ny+1; j<(irank+1)*ny+1; j++)
                for(i=1;i<NX;i++)
                    u[j][i] = un[j][i];
        } // end for(k)
    }   // end parallel

    if( (udata = fopen("u_mpilike.data","w")) == NULL ){
        printf("Can't open file.\n");
    }
    for(j=0;j<=NY;j++){//ファイル出力
        for(i=0;i<=NX;i++)
            fprintf( udata, "%.15E %.15E %.15E\n", i*h, j*h, u[j][i] );
        fprintf(udata,"\n");
    }
    fclose(udata);

    t2 = omp_get_wtime();
    printf("t2-t1: %f\n", t2-t1);

    return 0;
}
