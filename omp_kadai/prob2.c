#include<stdio.h>
#include<stdlib.h>

#define N 10

int main(int ac, char* av){
    int i,j;
    int a[N+1],b[N+1],c[N+1][N+1],f[N+1],x[N+1],y[N+1];
    int sum;
    int num_threads = omp_get_num_threads();
    int myid;
    
    // (0)
    for(i=0;i<N+1;i++)
        a[i]=b[i]=f[i]=x[i]=y[i] =i;
    printf("MAX threads:%d\n", omp_get_max_threads());
#pragma single
    {
        for(i=0;i<N;i++)
            printf("i:%d a:%d b:%d x:%d\n", i, a, b, x);
    }
#pragma omp parallel private(myid)
    {
        // (1)
        // a[1] = b[0] + c[1];
        // for(i=2;i<N;i++){
        //   b[i-1] = f[i-1];
        //   a[i] = b[i-1] +c[i];
        // }
           
        myid = omp_get_thread_num();
#pragma omp for schedule(static,2)
        for(i=1;i<N;i++){
            b[i] = f[i];
            a[i] = b[i-1] + x[i];
        }
    
        printf("\n\n");
        // (2)
        // jで並列化はダメ(依存)
#pragma omp for private (j)
        for(i=0;i<N;i++)
            for(j=0;j<N;j++)
                x[i] = x[i] + c[i][j] + b[j];

        // (3)
        // schedule(static,1) //fortranなので1,3,5がthread0

//  myid = 1 - myid;
        for(i=myid+1; i<N; i+=2){
            printf("i: %d, myid:%d\n", i, myid);
            a[i] = b[i];
        }

        // (4)
#pragma omp for schedule(static,1)
        for(i=2;i<N;i++)
            a[i] = a[i-2] + b[i];

        // (5)
        sum=0;
#pragma omp for reduction(+:sum)
        for(i=0;i<N;i++)
            sum = sum+a[i]*b[i];

        // (6) Stencilの計算
#pragma omp for private(j)
        for(i=0;i<N;i++)
            for(j=1;j<N;j++)
                c[i][j] = c[i][j] + c[i][j-1];
    } // END OMP PARALLEL


    return 0;
}
