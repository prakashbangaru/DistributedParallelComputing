#include <stdlib.h>
#include <stdio.h>
#include "timing.h"


int main(int argc, const char* argv[]){
    timing_start();
    long iter=0;
    long i=0;
    double pi=0.0;
    int numberOfThread=0;
    double count=0.0;
    double x=0.0;
    double y=0.0;
    double z=0.0;
    struct drand48_data drandBuf;
	
    if(argc!=2){
        printf("./MonteCarloSerial.out <numberOfDistributionPoints>\n");
        exit(-1);
    }

    iter = atoi(argv[1]);

    for ( i=0; i<iter ; i++) {
        drand48_r(&drandBuf, &x);   
        drand48_r(&drandBuf, &y);
        z = (x*x)+(y*y);
        if (z<=1.0){
            count=count+1.0;
         }
    }
    pi=(count*4.00/(double)iter);    
    printf("No.of distributed Points= %ld , Estimated Pi Value is %f \n", iter,pi);

    timing_stop();
    print_timing();
    return 0;
}
