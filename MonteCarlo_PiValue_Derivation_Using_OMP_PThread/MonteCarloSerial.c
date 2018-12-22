#include <stdlib.h>
#include <stdio.h>
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
    double count=0.0;
    double x=0.0;
    double y=0.0;
    double z=0.0;
	
    if(argc!=2){
        printf("./MonteCarloSerial.out <numberOfDistributionPoints>\n");
        exit(-1);
    }

    iter = atoi(argv[1]);
    srand(time(NULL));

    for ( i=0; i<iter ; i++) {
        x = getRand(0.0,1.0);
        y = getRand(0.0,1.0);
        //x = 0.5;
        //y = 0.5;
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


