#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include"timing.h"
#include<omp.h>

void convert(double** matrixA, double** matrixB,double* flatA,double *flatB,int dimension,int numberOfThread);
void optimizedParallelMultiplication(double** matrixC,double* flatA,double *flatB, int dimension,int numberOfThread);
void getMatrixInput(double **matrixValue, int size);
void displayMatrix(double **matrixValue,  int actualSize);
void readMatrix(char *inputFileName, double **matrix, int32_t matrixSize);


int main(int argc, char *argv[]){
	
	int matrixTotalSize=0;
	int matrixSize=0;
	int numberOfThread=0;
	double *flatA=NULL;
	double *flatB =NULL;
	double** matrixA=NULL;
	double** matrixB=NULL;
	double** matrixC=NULL;
        int row =0;
	
	if(argc <2){
		printf("matrixMultiplicationSerial.c <matrixSize>\r\n");
		exit(1);
	}
	
	matrixSize = atoi(argv[1]);
	matrixTotalSize = matrixSize*matrixSize;
	
	flatA = (double*)malloc(sizeof(double)*matrixTotalSize);
	memset(flatA,0,sizeof(double)*matrixTotalSize);
	
	flatB = (double*)malloc(sizeof(double)*matrixTotalSize);
	memset(flatB,0,sizeof(double)*matrixTotalSize);
	
	matrixA = malloc(matrixSize * sizeof(double*));
    if(matrixA==NULL){
            printf("\r\nMemory Allocation Failed at matrixA");
            exit(1);
    }
    memset(matrixA,0,matrixSize*sizeof(double*));
	
    for(row=0;row<matrixSize;row++){
        matrixA[row] = malloc(matrixSize*sizeof(double));
        memset(matrixA[row],0,matrixSize*sizeof(double));
        if(matrixA[row]==NULL){
            printf("\r\nMemory Allocation Failed at A");
            exit(1);
        }
    }

    matrixB = malloc(matrixSize * sizeof(double*));
    if(matrixB==NULL){
            printf("\r\nMemory Allocation Failed at matrixA");
            exit(1);
    }
    memset(matrixB,0,matrixSize*sizeof(double*));
	
    for(row=0;row<matrixSize;row++){
        matrixB[row] = malloc(matrixSize*sizeof(double));
        memset(matrixB[row],0,matrixSize*sizeof(double));
        if(matrixB[row]==NULL){
            printf("\r\nMemory Allocation Failed at A");
            exit(1);
        }
    }

    matrixC = malloc(matrixSize * sizeof(double*));
    if(matrixC == NULL){
            printf("\r\nMemory Allocation Failed at matrixC");
            exit(1);
    }
    memset(matrixC,0,matrixSize*sizeof(double*));
	
    for(row=0;row<matrixSize;row++){
        matrixC[row] = malloc(matrixSize*sizeof(double));
        memset(matrixC[row],0,matrixSize*sizeof(double));
        if(matrixC[row]==NULL){
            printf("\r\nMemory Allocation Failed at C");
            exit(1);
        }
    }
	
	//getMatrixInput(matrixA,matrixSize);
        //getMatrixInput(matrixB,matrixSize);
	readMatrix("inputMatrixA.txt", matrixA, matrixSize);
	readMatrix("inputMatrixB.txt", matrixB, matrixSize);

	timing_start();
	convert(matrixA, matrixB, flatA,flatB,matrixSize,numberOfThread);
	optimizedParallelMultiplication(matrixC,flatA,flatB,matrixSize,numberOfThread);
	timing_stop();
        print_timing();	

        displayMatrix(matrixC,matrixSize);	

	for(row=0;row<matrixSize;row++){
		free(matrixA[row]);
		free(matrixB[row]);
		free(matrixC[row]);
        }	

        free(matrixA);
        free(matrixB);
	free(matrixC);

        free(flatA);
	free(flatB);
	return 0;
}

void displayMatrix(double **matrixValue, int actualSize){
     int row=0;
     int column=0;
     FILE *fp=NULL;
     char comma[2];

     fp = fopen("serailOutputMatrix.txt", "w");
     fprintf(fp, "\nResultant Matrix Size = %d\n", actualSize);

     for(row = 0; row < actualSize; row++){
        sprintf(comma, "%s", "");
        for(column = 0; column < actualSize; column++){
                  fprintf(fp, "%s%f",comma,matrixValue[row][column]);
                  sprintf(comma, "%s", " ");
         }
         fprintf(fp, "\n");
    }
}

void readMatrix(char *inputFileName, double **matrix, int32_t matrixSize){
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



void getMatrixInput(double **matrixValue,int size){
  FILE *fp=NULL;
  int row=0;
  int column=0;
  char comma[2];
  double value=0.0;
  struct drand48_data drandBuf; 
  fp = fopen("serialMatrixData.csv", "a");
  fprintf(fp, "%d\n", size);

  for(row=0;row<size;row++){
        sprintf(comma, "%s", "");
        for(column=0;column<size;column++){
            drand48_r(&drandBuf, &value);
            matrixValue[row][column]=value;
            fprintf(fp, "%s%f",comma,value);
            sprintf(comma, "%s", ",");
        }
        fprintf(fp, "\n");
   }
  fclose(fp);
}




/*The below function converts 2 dimensional input matrices into row major and column major 1 dimensional matrices*/
/*This Conversion helps in memory high Jump and reduces the memory access time*/
void convert(double** matrixA, double** matrixB,double *flatA, double *flatB,int dimension,int numberOfThread){
      int i=0;
      int j=0;

      for( i=0; i<dimension;i++){
	      for(j=0; j<dimension;j++){
		flatA[i*dimension+j]=matrixA[i][j];   // row Major Matrix
	        flatB[j*dimension+i]=matrixB[i][j];   // Column Matrix
          }			  
      }		  
}

void optimizedParallelMultiplication(double** matrixC,double *flatA, double *flatB,int dimension,  \
                                              int numberOfThread){

	int i=0;
	int j=0;
	int k=0;
	int iOff=0;
	int jOff=0;
	double tot=0.0;

	for(i=0; i<dimension; i++){
		iOff = i * dimension;
		for(j=0; j<dimension; j++){
			jOff = j * dimension;
			tot = 0;
			for(k=0; k<dimension; k++){
			   tot += flatA[iOff + k] * flatB[jOff + k];
			}
			 matrixC[i][j] = tot;
		 }
	}
}
