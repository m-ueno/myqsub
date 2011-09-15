#include<stdio.h>
#include<stdlib.h>

int main(void){
	int i,n;
	double w, gsum, pi, v;

	n = 2000000;
	w = (double)1.0/n;
	gsum = 0.0;

#pragma omp parallel
	{
#pragma omp for private(v) reduction(+:gsum)
		for(i=0; i<n; i++){
			v = w * (i+(double)0.5);
			v = (double)4.0 / ((double)1.0 + v * v);
			gsum = gsum + v;
		}
	}

	pi = gsum * w;
	printf("%s%.15g\n", "PI is ", pi);

	return 0;
}
