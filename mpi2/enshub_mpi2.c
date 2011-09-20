#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

#define NX 193
#define NY 193
#define LW 67
#define MCW MPI_COMM_WORLD

int main(int argc, char** argv){
    int irank, nrank;
    MPI_Init (&argc, &argv);
    MPI_Comm_size (MCW, &nrank);
    MPI_Comm_rank (MCW, &irank);

    int nx, ny;
    int px, py;

    /* コピペゾーン */
    // (1) init dims
    int dims[2] = {0,0};
    MPI_Dims_create(nrank,2,dims);
    nx = (NX-1)/dims[0];
    ny = (NY-1)/dims[1];
    
    // (2) init cart
    int periods[2] = {0,0}; // 非周期境界
    MPI_Comm cart;
    MPI_Cart_create(MCW, 2, dims, periods, 0, &cart);

    int c[2];
    MPI_Cart_coords(cart, irank, 2, c);
    py = c[0]; // c[2]は大きい順なのでyがc[0]
    px = c[1];

    double h = 1.0/NX;
    double dt = 0.1*h*h;
    double dth2 = dt/h/h;
    int i,j,k;
    int height = ny+2, width = nx+2;

    double (*u)[width];
    u = (double(*)[width])malloc(height*width*sizeof(double));
    u = (double(*)[width])(&u[1][1]);

    double (*un)[width];
    un = (double(*)[width])malloc(height*width*sizeof(double));
    un = (double(*)[width])(&un[1][1]);

    for (j=-1;j<ny+1;j++)
        for (i=-1;i<nx+1;i++)
            u[j][i] = 0.0;
    // (y=0)
    if (py==0)
        for (i=-1;i<nx+1;i++)
            u[-1][i] = 1.0;

    // (x=0)
    if (px==0)
        for (j=0;j<ny+1;j++)
            u[j][-1] = 0.5;

    MPI_Datatype vedge;
    MPI_Type_vector(ny, 1, nx+2, MPI_DOUBLE, &vedge);
    MPI_Type_commit(&vedge);
    int north, south, east, west;
    /* loop start */
    for (k=0; k<400; k++){
        for (j=0; j<ny; j++){
            for (i=0; i<nx; i++)
                un[j][i] = u[j][i] + ( -4*u[j][i] + u[j][i+1] + u[j][i-1] + u[j+1][i] + u[j-1][i] )*dth2;
        }
        for (j=0; j<ny; j++){
            for (i=0; i<nx; i++)
                u[j][i] = un[j][i];
        }
        MPI_Cart_shift(cart,0,1,&south,&north);
        MPI_Cart_shift(cart,1,1,&west,&east);

        MPI_Sendrecv(&u[ny-1][0], nx, MPI_DOUBLE, north, 0,
                     &u[-1][0], nx, MPI_DOUBLE, south, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv(&u[0][0], nx, MPI_DOUBLE, south, 0,
                     &u[ny][0], nx, MPI_DOUBLE, north, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        MPI_Sendrecv(&u[0][nx-1], 1, vedge, east, 0,
                     &u[0][-1], 1, vedge, west, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Sendrecv(&u[0][0], 1, vedge, west, 0,
                     &u[0][nx], 1, vedge, east, 0,
                     MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    } // end loop(k)

    /* PRINT */

    /* 西端にgatherしてprint */
    MPI_Comm row;
    MPI_Cart_coords(cart, irank, 2, c);
    MPI_Comm_split(cart, py, px, &row);

// MPI_Gather (&sendbuf,sendcnt,sendtype,&recvbuf, 
//             recvcount,recvtype,root,comm)
    double recvbuf[(ny+2)*dims[0]][(nx+2)];
    printf("rank: %d, c: %d %d\n", irank, c[1], c[0]);
    MPI_Gather(&u[-1][-1], (nx+2)*(ny+2), MPI_DOUBLE,
               recvbuf, (nx+2)*(ny+2), MPI_DOUBLE, 0, row);

    FILE *udata;
        /* output: rank0 */
    if(irank==3){
        udata = fopen("u.data","w");
        for(j=-1;j<ny+1;j+=4){
            for(i=-1;i<nx+1;i+=4)
                fprintf( udata, "%.15E %.15E %.15E\n", (i+1)*h, (j+1)*h, u[j][i] );
            fprintf( udata, "\n" );
        }
        fclose(udata);
    }
    
/*
    for(int pj=0; pj<dims[0]; pj++){
        if(px==0){
            udata = fopen("u.data","w");
            for(j=0; j<ny; j+=4){
                for(int pi=0; pi<dims[1]; pi++)
                    for(i=0; i<nx; i+=4)
                        fprintf( udata, "%.15E %.15E %.15E\n", (i+1 + nx*pi)*h, (j+1 + ny*pj)*h, u[j + pi*ny][i] );
                fprintf( udata, "\n" );
            }
            fclose(udata);
        } //end if px==0
        MPI_Barrier(MCW);
    }
*/
//    MPI_File_close(&udata);
    MPI_Finalize ();

    return 0;
}
