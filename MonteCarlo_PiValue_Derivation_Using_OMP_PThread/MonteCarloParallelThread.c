#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timing.h"


typedef struct{
long numberOfTask; // It is Number Of Points
long circularPointCount;
}Task;



double getRand(double min, double max){
	double d  = (double)(rand() / (RAND_MAX/(max - min))) + min;
        return d;
}

void threadRun(void *arg){
   
     Task *task = (Task*)arg;
     double count=0.0;
     double x=0.0;
     double y=0.0;
     long i=0;
     struct drand48_data drandBuf;

     task->circularPointCount=0; 
     
       for (i=0; i < task->numberOfTask ; i++) {
         drand48_r(&drandBuf, &x);   
         drand48_r(&drandBuf, &y); 
        if (((x*x)+(y*y))<=1.0){
           task->circularPointCount++;
         }
       }
}


int main(int argc, const char* argv[]){
    timing_start();
    long iter=0;
    long i=0;
    double pi=0.0;
    int numberOfThread=0;
    long circularCount=0;
    int numberOfTask=0;
    int remainingTask=0;
    int index=0;
    int join_error=0;
    int create_error=0;
	
    if(argc <3){
        printf("./MonteCarloParallel.out <numberOfDistributionPoints> <numberOfThreads>\n");
        exit(-1);
    }

    iter = atoi(argv[1]);
    numberOfThread = atoi(argv[2]);
    if(numberOfThread==0){
        printf("./MonteCarloParallel.out <numberOfDistributionPoints> <numberOfThreads>\n");
        printf("Number Of Threads should be greater than 1\n");
         exit(-1);
    }

    pthread_t threads[numberOfThread];
    Task task[numberOfThread];

   // memset(task,0,sizeof(Task)*numberOfThread);

    numberOfTask = iter / numberOfThread;
    remainingTask = iter % numberOfThread;

    for(index=0;index<numberOfThread;index++){
       task[index].numberOfTask=numberOfTask;
       if(remainingTask){
           task[index].numberOfTask++;
           remainingTask--;
       }
    }

    srand(time(NULL));
    
    for(index=0;index<numberOfThread;index++){
        create_error = pthread_create(&threads[index], NULL, threadRun, &task[index]);
        if(create_error){
                printf("pthread_create returned error: %d\n", create_error);
                exit(-1);
        }
    }

    for(index=0;index<numberOfThread;index++){
        join_error = pthread_join(threads[index], NULL);
        if(join_error){
                printf("pthread_join returned error: %d\n", join_error);
                exit(-1);
        }
    } 
  
   

    for(index=0;index<numberOfThread;index++){
       circularCount+=task[index].circularPointCount;
    }

       pi=((double)circularCount*4.00/(double)iter);    
       printf("No.of distributed Points= %ld , Estimated Pi Value is %f \n", iter,pi);

    timing_stop();
    print_timing();
    pthread_exit(NULL);
    return 0;
}


