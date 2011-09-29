/* MPI 1次元領域分割 */
#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define NX 193
#define NY 193

int main(int argc, char** argv){
    int irank,nrank;

    MPI_Init (&argc, &argv);
    MPI_Comm_size (MPI_COMM_WORLD, &nrank);
    MPI_Comm_rank (MPI_COMM_WORLD, &irank);

    double t1,t2;
    if(irank==0) t1 = MPI_Wtime();

    int height=(NY-1)/nrank+2, width=NX+1;
    
    double (*u)[width];
    u = (double(*)[width])malloc(height*width*sizeof(double));
    u = (double(*)[width])(&u[1][1]);

    double (*un)[width];
    un = (double(*)[width])malloc(height*width*sizeof(double));
    un = (double(*)[width])(&un[1][1]);

    double h = 1.0/NX;
    double dt = 0.1*h*h;
    double dth2 = dt/h/h;
    FILE *udata;
    int i,j,k;

    for(j=-1;j<height-1;j++)
        for(i=-1;i<NX;i++)
            u[j][i] = 0.0;
    // (y=0)
    if( irank==0 )
        for(i=-1;i<NX;i++)
            u[-1][i] = 1.0;

    // (x=0)
    for(j=0;j<height-1;j++)
        u[j][-1] = 0.5;

    int north = irank<nrank-1 ? irank+1 : MPI_PROC_NULL; // upper
    int south = irank>0 ? irank-1 : MPI_PROC_NULL;       // lower
    /* loop start */
    for(k=0;k<40000;k++){
        for(j=0;j<height-2;j++)
            for(i=0;i<NX-1;i++)
                un[j][i] = u[j][i] + ( -4*u[j][i] + u[j][i+1] + u[j][i-1] + u[j+1][i] + u[j-1][i] )*dth2;
        for(j=0;j<height-2;j++){
            for(i=0;i<NX-1;i++){
                u[j][i] = un[j][i];
            }
        }
        /* sendrecv */
        if(nrank>1){
            MPI_Sendrecv(&u[height-3][0], width-2, MPI_DOUBLE, north, 0,
                         &u[-1][0] , width-2, MPI_DOUBLE, south, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Sendrecv(&u[0][0], width-2, MPI_DOUBLE, south, 0,
                         &u[height-2][0], width-2, MPI_DOUBLE, north, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    } // end loop(k)

    /* PRINT */
    if (irank == 0){
        udata = fopen("u.data","w");
        /* output: rank0 */
        for(j=-1;j<height-2;j++){
            for(i=-1;i<NX;i++)
                fprintf( udata, "%.15E %.15E %.15E\n", (i+1)*h, (j+1)*h, u[j][i] );
            fprintf( udata, "\n" );
        }
        /* output: rank1 ~ n-2 */
        for(int jrank=1; jrank<nrank-1; jrank++){
            //MPI_Recv (&buf,count,datatype,source,tag,comm,&status)
            MPI_Recv( &u[0][-1], width*(height-2), MPI_DOUBLE, jrank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
            for(j=0;j<height-2;j++){
                for(i=-1;i<NX;i++)
                    fprintf( udata, "%.15E %.15E %.15E\n", (i+1)*h, ( j+1+jrank*(height-2) )*h, u[j][i] );
                fprintf( udata, "\n" );
            }
        }
        /* output: rank_n-1 */
        if(nrank>0){
        MPI_Recv( &u[0][-1], width*(height-1), MPI_DOUBLE, nrank-1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE );
        for(j=0;j<height-1;j++){
            for(i=-1;i<NX;i++)
                fprintf( udata, "%.15E %.15E %.15E\n", (i+1)*h, ( j+1+(nrank-1)*(height-2) )*h, u[j][i] );
            fprintf( udata, "\n" );
        }
        }        
        fclose(udata);
    } else if (irank<nrank-1) {
        MPI_Send (&u[0][-1], width*(height-2), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD );
    } else {
        MPI_Send (&u[0][-1], width*(height-1), MPI_DOUBLE, 0, 0, MPI_COMM_WORLD );
    }

    if (irank==0) {
        t2 = MPI_Wtime();
        printf("\ntime: %g\n",t2-t1);
    }

    MPI_Finalize ();

    return 0;
}
