#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include<omp.h>
#include"timing.h"
#include "openmpi-x86_64/mpi.h"
//#include <mpi.h>


void convert(double** matrixB,double *flatB,int dimension,int numberOfThread);
void getMatrixInput(double **matrixValue, int size);
void displayMatrix(double *matrixValue,  int totalDataSize, int matrixSize);
void printResultMatrix(double *flatC,int blockSize);
void optimizedParallelMultiplication(double* flatC,double *flatA, double *flatB, \
                                      int blockSize, int dimension,int numberOfThread);
void readMatrixB(char *inputFileName, double **matrix, int matrixSize);
//void readMatrixA(char *inputFileName, double *matrix, int matrixSize);
void readMatrixA(char *inputFileName, double *flatA, int32_t matrixSize, int32_t skipCount,int32_t totalDataCount );

typedef struct {
double *flatC;
}NodeDataType;


int main(int argc, char *argv[]){
	
	int32_t matrixTotalSize=0;
	int32_t matrixSize=0;
	int32_t numberOfThread=0;
	double *flatA=NULL;
	double *flatB =NULL;
	double *flatC =NULL;
	double** matrixA=NULL;
	double** matrixB=NULL;
	int32_t row =0;
	int32_t rank=0;
	int32_t processRank=0;
	int32_t equalBlockSize =0;
	int32_t remainingBlockSize=0;
        int32_t totalDataSize=0;
	int32_t nodeTotalMatrixSize=0;
	int32_t beginPointer=0;
	int32_t np=0;
    int32_t masterResultDataSize = 0;
    int32_t slaveResultDataSize=0;

    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&np);
    timing_start();	
	
    
	if(argc <3){
		printf("parallelMultiplicationMpi.c <matrixSize> <numberofTheread>\r\n");
		exit(1);
	}
	
	matrixSize = atoi(argv[1]);
        numberOfThread = atoi(argv[2]);
	matrixTotalSize = matrixSize*matrixSize;
        equalBlockSize = matrixSize/np;
	totalDataSize = equalBlockSize*matrixSize; /*Ouput Data count from each node except the Master*/
        
        remainingBlockSize = matrixSize%np;
	masterResultDataSize = (equalBlockSize+remainingBlockSize)*matrixSize;
	slaveResultDataSize = equalBlockSize*matrixSize;
        matrixB = malloc(matrixSize * sizeof(double*));
		if(matrixB==NULL){
            	printf("\r\nMemory Allocation Failed at matrixB");
            	exit(1);
    	}
    	memset(matrixB,0,matrixSize*sizeof(double*));
	
    	for(row=0;row<matrixSize;row++){
        	matrixB[row] = malloc(matrixSize*sizeof(double));
        	memset(matrixB[row],0,matrixSize*sizeof(double));
        		if(matrixB[row]==NULL){
            			printf("\r\nMemory Allocation Failed at matrixB");
            			exit(1);
        		}
    	}
		
        flatB = (double*)malloc(sizeof(double)*(matrixTotalSize));
    	if(flatB==NULL){
            	printf("\r\nMemory Allocation Failed at flatB");
            	exit(1);
    	}
	    memset(flatB,0,sizeof(double)*(matrixTotalSize));
        
	readMatrixB("./inputMatrixB.txt",matrixB,matrixSize);
        convert(matrixB,flatB,matrixSize,numberOfThread);


        /*Since we got Column Majore Vector Matrix we no need 2 array Matrix further*/        
        #pragma omp for schedule(static)
        for(row=0;row<matrixSize;row++){
	     free(matrixB[row]);
        }	
        free(matrixB);


        if(rank==0)
        {
        	flatA = (double*)malloc(sizeof(double)*(masterResultDataSize));
    		if(flatA==NULL){
            	printf("\r\nMemory Allocation Failed at flatA");
            	exit(1);
    		}
		    memset(flatA,0,sizeof(double)*(masterResultDataSize));
	
		NodeDataType nodeData[np];
		memset(nodeData,0,sizeof(NodeDataType)*np);
		
		nodeData[0].flatC = (double*)malloc(sizeof(double)*masterResultDataSize);
	         memset(nodeData[0].flatC,0,sizeof(double)*masterResultDataSize);
		
		/*Memeory Allocation to store the Slave Result*/
	    for (processRank= 1; processRank < np; processRank++){
		         nodeData[processRank].flatC = (double*)malloc(sizeof(double)*slaveResultDataSize);
	                 memset(nodeData[processRank].flatC,0,sizeof(double)*slaveResultDataSize);
    	   }
		   
            readMatrixA("inputMatrixA.txt",flatA,matrixSize,0,masterResultDataSize);

	    /*Master Computes the Matrix Multiplication with its corresponding data*/
	    optimizedParallelMultiplication(nodeData[0].flatC,flatA,flatB,equalBlockSize+remainingBlockSize,matrixSize,numberOfThread);
		
            //#pragma omp parallel for private(processRank,status)
	    for (processRank=1; processRank<np; processRank++){
             MPI_Recv(nodeData[processRank].flatC,slaveResultDataSize, MPI_DOUBLE, processRank, 1, MPI_COMM_WORLD, &status);
            }

        /*Master Writes its computed Output Data to the File*/
        displayMatrix(nodeData[0].flatC,masterResultDataSize,matrixSize);
		
        /*Master Writes Slave computed Output Data to the File*/
	    for (processRank=1; processRank<np; processRank++){
          displayMatrix(nodeData[processRank].flatC,slaveResultDataSize,matrixSize);
        }
        
         
        //#pragma omp parallel for private(processRank,status) schedule(static)  num_threads(np)
        #pragma omp parallel for private(processRank,status)
	   for (processRank=0; processRank<np; processRank++){
	        free(nodeData[processRank].flatC);
        }
        
	 timing_stop();
         print_timing();	
	
   }
    if(rank>0)
    { 

	double *slaveFlatC=NULL;
        int32_t skipCount;
	
        
		flatA = (double*)malloc(sizeof(double)*(slaveResultDataSize));
    	        if(flatA==NULL){
            	    printf("\r\nMemory Allocation Failed at flatA");
            	    exit(1);
    	        }
		memset(flatA,0,sizeof(double)*(slaveResultDataSize));
	    
		skipCount = (rank*equalBlockSize*matrixSize)+ remainingBlockSize;
	        readMatrixA("inputMatrixA.txt",flatA,matrixSize,skipCount,slaveResultDataSize);
		
        slaveFlatC = (double*)malloc(sizeof(double)*(totalDataSize));
    	if(slaveFlatC==NULL){
            	printf("\r\nMemory Allocation Failed at slaveFlatC");
            	exit(1);
    	}
	memset(slaveFlatC,0,sizeof(double)*(totalDataSize));
        
        optimizedParallelMultiplication(slaveFlatC,flatA,flatB,equalBlockSize,matrixSize,numberOfThread);
        MPI_Send(slaveFlatC,totalDataSize, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        free(slaveFlatC);
    	
    }
    free(flatA);
    free(flatB);
    MPI_Finalize();	
    return 0;
}

void displayMatrix(double *matrix, int totalDataSize,int matrixSize){
     int i=0;
     FILE *fp=NULL;
     char comma[2];

     fp = fopen("outputMatrixFile.txt", "a");

        sprintf(comma, "%s", "");
        fprintf(fp, "%s%lf",comma,matrix[i]);
        sprintf(comma, "%s", " ");

        for(i = 1; i <totalDataSize; i++){
                  if(i%matrixSize ==0){
                    fprintf(fp, "\n");
                    sprintf(comma, "%s", "");
                  }
                  fprintf(fp, "%s%lf",comma,matrix[i]);
                  sprintf(comma, "%s", " ");
         }
    fprintf(fp, "\n");
    fclose(fp);
}

#if 0
void readMatrixA(char *inputFileName, double *flatA, int32_t matrixSize){
   FILE *file = fopen(inputFileName, "r");
       if (file){
            int32_t i=0, j=0;
            int32_t totalMatrixSize = matrixSize*matrixSize;
           
            for(i=0; i < totalMatrixSize;i++){
                if (!fscanf(file, "%lf", &flatA[i])) 
                    break;
            }
            fclose(file);
        }
}
#endif

void readMatrixA(char *inputFileName, double *flatA, int32_t matrixSize, int32_t skipCount,int32_t totalDataCount ){

   FILE *file = fopen(inputFileName, "r");
       if (file){
            int32_t i=0, j=0;
            int32_t totalMatrixSize = matrixSize*matrixSize;
            char buffer[BUFSIZ];
            double tempVar =0.0;
            
			/*skip the File Content of Other Processor*/
			for(i=0; i < totalMatrixSize;){
				if(i<skipCount){
                   if(fgets(buffer, BUFSIZ,file)){
                          i = i+matrixSize;
						   continue;
                    }
                    else{
						fclose(file);
						printf("Not Correct Input FileSystem Exits\n");
                        exit(EXIT_FAILURE);
					}
                } 
				else 
					break;
            }
			
			/*Read the File content of the current Process*/
			for(i=0; i < totalDataCount;i++){
                   if (!fscanf(file, "%lf", &flatA[i]))
                     break;
             }
            fclose(file);
	   }
}

void readMatrixB(char *inputFileName, double **matrix, int32_t matrixSize){
   FILE *file = fopen(inputFileName, "r");
        if (file){
            int32_t i=0, j=0;
            
            for ( i = 0; i< matrixSize; ++i ){
                for ( j = 0; j < matrixSize ; ++j){
                    if (!fscanf(file, "%lf", &matrix[i][j])) 
                          break;
                }
            }
            fclose(file);
        }
}



/*The below function converts 2 dimensional input matrices into row major and column major 1 dimensional matrices*/
/*This Conversion helps in reduce memory high Jump and reduces the memory access time*/
//void convert(double** matrixA, double** matrixB,double *flatA, double *flatB,int dimension,int numberOfThread){
void convert(double** matrixB, double *flatB,int dimension,int numberOfThread){
      int32_t i=0;
      int32_t j=0;

   #pragma omp parallel for private(i, j) schedule(static)  num_threads(numberOfThread)
      for( i=0; i<dimension;i++){
	      for(j=0; j<dimension;j++){
	        flatB[j*dimension+i]=matrixB[i][j];   // Column Matrix
          }			  
      }		  
}

void optimizedParallelMultiplication(double* flatC,double *flatA, double *flatB,int32_t blockSize, int32_t dimension,int32_t numberOfThread)
{
	int32_t i=0;
	int32_t j=0;
	int32_t k=0;
	int32_t iOff=0;
	int32_t jOff=0;
	double tot=0.0;
	#pragma omp parallel shared(flatC) private(i, j, k, iOff, jOff, tot) num_threads(numberOfThread)
	{
		#pragma omp for schedule(static)
		for(i=0; i< blockSize; i++){
			iOff = i * dimension;
			for(j=0; j< dimension; j++){
				jOff = j * dimension;
				tot = 0;
				for(k=0; k<dimension; k++){
					tot += flatA[iOff + k] * flatB[jOff + k];
				}
				flatC[(i*dimension)+j] = tot;
			}
		}
	}
}

void printResultMatrix(double *flatC,int blockSize){

  int i=0;
  for(i=0;i<blockSize;i++){
      printf("%lf\n",flatC[i]);
  }
}
