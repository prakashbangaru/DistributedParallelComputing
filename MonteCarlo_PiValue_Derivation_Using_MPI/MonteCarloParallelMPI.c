#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "timing.h"
#include "openmpi-x86_64/mpi.h"
//#include <mpi.h>


typedef struct{
int circularPointCount;
}NodeDataType;

int getNumberOfCircularPointCounts(int numberOfDistributionPoints){
   
     double count=0.0;
     double x=0.0;
     double y=0.0;
     long i=0;
     struct drand48_data drandBuf;
	 int circularPointCounts=0;
	 
       //#pragma omp parallel for schedule(auto)  private(x,y) num_threads(numberOfThread)   
       for (i=0; i < numberOfDistributionPoints; i++) {
	/*drand48_r function randomly Generates the Value between 0 and 1*/
         drand48_r(&drandBuf, &x);   
         drand48_r(&drandBuf, &y); 
         if (((x*x)+(y*y))<=1.0){
             circularPointCounts++;
          }
       }
	   return circularPointCounts;
}


int main(int argc,char* argv[]){
    
    double pi=0.0;
    int numberOfThread=0;
    int numberOfTask=0;
    int remainingTask=0;
    int numberOfProcess=0;
    int rank =0;
    int masterProcessDistributionCount=0;
    int slaveProcessDistributionCount=0;
    int totalDistributionCount=0;
    int circularPointCount=0;
    int totalCircularPointCount=0;
    int processRank=0;
    int remainingDistributionCount =0; 
	
	
	
    if(argc <2){
        printf("./part_7_parallel.out <numberOfDistributionPoints>\n");
        exit(-1);
    }

    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numberOfProcess);
    
    timing_start();
    totalDistributionCount = atoi(argv[1]);
	
    NodeDataType nodeData[numberOfProcess];
    memset(nodeData,0,sizeof(NodeDataType)*numberOfProcess);
	
	slaveProcessDistributionCount = totalDistributionCount / numberOfProcess;
	remainingDistributionCount = totalDistributionCount % numberOfProcess;
	masterProcessDistributionCount = slaveProcessDistributionCount + remainingDistributionCount; 
   
    
	if(rank == 0){
	
        nodeData[rank].circularPointCount = getNumberOfCircularPointCounts(masterProcessDistributionCount);
		
	//#pragma omp parallel for schedule(auto)  private(processRank) num_threads(numberOfProcess-1)
	for (processRank=1; processRank<numberOfProcess; processRank++){
             MPI_Recv(&nodeData[processRank].circularPointCount,1, MPI_INT, processRank, 1, MPI_COMM_WORLD, &status);
             //printf("Received circularpoint %d from rank %d\n",nodeData[processRank].circularPointCount,processRank);
        }
		
	for (processRank=0; processRank<numberOfProcess; processRank++){
            totalCircularPointCount+=nodeData[processRank].circularPointCount;
        }
		
	pi=((double)totalCircularPointCount*4.00/(double)totalDistributionCount);    
        printf("No.of distributed Points= %d , Estimated Pi Value is %lf \n", totalDistributionCount,pi);
        timing_stop();
        print_timing();
	}
	
	if(rank > 0){
		circularPointCount = getNumberOfCircularPointCounts(slaveProcessDistributionCount);
		MPI_Send(&circularPointCount,1, MPI_INT, 0, 1, MPI_COMM_WORLD);
                //printf("Sent circularpoint %d from rank %d\n",circularPointCount,rank);
	}
	
	MPI_Finalize();
    return 0;
}
