#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "timing.h"

double getRand(double min, double max){
	double d  = (rand() / (RAND_MAX/(max - min))) + min;
        return d;
}

int main(int argc, const char* argv[]){
    timing_start();
    long iter=0;
    long i=0;
    double pi=0.0;
    int numberOfThread=0;
    long count=0;
    struct drand48_data drandBuf;

    if(argc!=3){
        printf("./MonteCarloParallelOMP.out <numberOfDistributionPoints> <numberOfThreads>\n");
        exit(-1);
    }

    iter = atoi(argv[1]);
    numberOfThread = atoi(argv[2]);

    srand(time(NULL));
    
    #pragma omp parallel private(i) num_threads(numberOfThread)
    {
        double x=0.0;
        double y=0.0;
       #pragma omp for schedule(static) reduction(+:count) 
       for ( i=0; i<iter ; i++) {
         drand48_r(&drandBuf, &x);   
         drand48_r(&drandBuf, &y); 
        if (((x*x)+(y*y))<=1.0){
            count++;
         }
       }
    }
       pi=((double)count*4.00/(double)iter);    
       printf("No.of distributed Points= %ld , Estimated Pi Value is %f \n", iter,pi);

    timing_stop();
    print_timing();
    return 0;
}


