/* written by Hibino */
#include<stdio.h>

#define NX 193
#define NY 193
#define ND ((NX+1)*(NY+1))
#define NXNY (NX*NY)

int main(int argc, char *argv[]){
    double u[NY+1][NX+1], un[NY+1][NX+1];
    double h = 1.0/193;
    double dt = 0.1*h*h;
    double dth2 = dt/(h*h);
    FILE *udata;
    int i,j,k;

    for(j=0;j<=NY;j++){         //配列の初期化
        for(i=0;i<=NX;i++){
            if(i==0){
                u[j][i] = 0.5;
                un[j][i] = 0.5;
            }else if(j==0){
                u[j][i] = 1.0;
                un[j][i] = 1.0;
            }else{
                u[j][i] = 0.0;
                un[j][i] = 0.0;
            }
        }
    }

    for(k=0;k<40000;k++){       // 40000 times loop
        for(j=1;j<NY;j++){
            for(i=1;i<NX;i++){
                un[j][i] = u[j][i] + ( -4*u[j][i] + u[j][i+1] + u[j][i-1] + u[j+1][i] + u[j-1][i] )*dth2;
            }
        }
        for(j=1;j<NY;j++){
            for(i=1;i<NX;i++){
                u[j][i] = un[j][i];
            }
        }
    }

    if((udata = fopen("u.data","w"))==NULL){
        printf("ファイルを開けません。\n");
    }
    for(j=0;j<=NY;j++){//ファイル出力
        for(i=0;i<=NX;i++){
            fprintf(udata,"%.15E %.15E %.15E\n",i*h,j*h,u[j][i]);
        }
        fprintf(udata,"\n");
    }
    fclose(udata);

    return 0;
}
