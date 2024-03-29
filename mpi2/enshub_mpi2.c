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

    double t1,t2;
    if(irank==0) t1 = MPI_Wtime();

    int nx, ny;
    int px, py;

    /* コピペゾーン */
    // (1) init dims
    int dims[2] = {0,0};
    MPI_Dims_create(nrank,2,dims);
    ny = (NY-1)/dims[0];
    nx = (NX-1)/dims[1];
    
    // (2) init cart
    int periods[2] = {0,0}; // 非周期境界
    MPI_Comm cart;
    MPI_Cart_create(MCW, 2, dims, periods, 0, &cart);

    int c[2];                   /* 座標 */
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
        for (i=-1;i<nx+1;i++){
            u[j][i] = 0.0;
        }
    // (y=0)
    if (py==0)
        for (i=-1;i<nx+1;i++){
            u[-1][i] = 1.0;
        }

    // (x=0)
    if (px==0)
        for (j=0;j<ny+1;j++){
            u[j][-1] = 0.5;
        }

    MPI_Datatype vedge;
    MPI_Type_vector(ny, 1, nx+2, MPI_DOUBLE, &vedge);
    MPI_Type_commit(&vedge);
    int north, south, east, west;
    MPI_Cart_shift(cart,0,1,&south,&north);
    MPI_Cart_shift(cart,1,1,&west,&east);

    /* loop start */
    for (k=0; k<40000; k++){
        for (j=0; j<ny; j++){
            for (i=0; i<nx; i++)
                un[j][i] = u[j][i] + ( -4*u[j][i] + u[j][i+1] + u[j][i-1] + u[j+1][i] + u[j-1][i] )*dth2;
        }
        for (j=0; j<ny; j++){
            for (i=0; i<nx; i++)
                u[j][i] = un[j][i];
        }

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

    /* setview */
    MPI_File udata;
    MPI_File_open(cart, "u.data", MPI_MODE_WRONLY | MPI_MODE_CREATE,
                  MPI_INFO_NULL, &udata);
    MPI_File_set_size(udata,0);

    int size[2] = {NY+1, LW*(NX+1)+1}, subsize[2], start[2];
// ... py,pxはcartesian座標
    subsize[0] = ny;
    subsize[1] = LW*nx;
    start[0] = py*ny+1;
    start[1] = LW*(px*nx+1);

    if (py == 0){ subsize[0]++; start[0]=0; }   /* 南端↓ */
    if (py == dims[0]-1) subsize[0]++;          /* 北端↑ */
    if (px == 0){ subsize[1]+=LW; start[1]=0; } /* 西端← */
    if (px == dims[1]-1) subsize[1]+=LW+1;      /* 東端→ */

    MPI_Datatype ftype;
    MPI_Type_create_subarray(2, size, subsize, start,
                             MPI_ORDER_C, MPI_CHAR, &ftype);
    MPI_Type_commit(&ftype);
    MPI_File_set_view(udata, 0, MPI_CHAR, ftype, "native",
                      MPI_INFO_NULL);

    /* output */
    MPI_Status st;
    char *wbuf = (char*)malloc((LW*(nx+2)+2)*sizeof(char));

    int jstart=0,istart=0, jend=ny, iend=nx;
    if(py==0) jstart = -1;
    if(py==dims[0]-1) jend = ny+1;
    if(px==0) istart = -1;
    if(px==dims[1]-1) iend = nx+1;

    for(j=jstart; j<jend; j++){
        for(i=istart,k=0; i<iend; i++,k+=LW){
            sprintf( wbuf+k, " %.15E %.15E %21.15E\n",
                     (i+1 + px*nx)*h, (j+1 + py*ny)*h, u[j][i] );
        }
        if( px == dims[1]-1 )     // 東端→
            sprintf(wbuf+(k++),"\n");
        MPI_File_write(udata,wbuf,k,MPI_CHAR,&st);
    }

    MPI_File_close(&udata);

    if(irank==0){
        t2 = MPI_Wtime();
        printf("%g\n",t2-t1);
    }

    MPI_Finalize ();

    return 0;
}
