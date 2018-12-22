#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include"timing.h"

#define MIN_MATRIX_SIZE 2

typedef struct{
double **matrixValue;
}Matrix;

double getRand(double min, double max){
    double d = (double)rand() / RAND_MAX;
    return min + d * (max - min);
}

void getMatrixInput(double **matrixValue,int size){
  FILE *fp=NULL;
  int row=0;
  int column=0;
  double min=0.00001;
  double  max=100000;
  char comma[2];
  double value=0.0;
 
  fp = fopen("inputMatrix.csv", "a");
  srand(time(NULL));
  fprintf(fp, "%d\n", size);

  for(row=0;row<size;row++){
  	sprintf(comma, "%s", "");
    	for(column=0;column<size;column++){
                        value= getRand(min,max);
                        matrixValue[row][column]=value;
           		fprintf(fp, "%s%f",comma,value);
           sprintf(comma, "%s", ",");
	}
        fprintf(fp, "\n");
   }
  fclose(fp);
}



void displayMatrix(double **matrixValue, int actualSize){
     int row=0;
     int column=0;
     FILE *fp=NULL;
     char comma[2];

     fp = fopen("inputMatrix.csv", "a");
     fprintf(fp, "\nResultant Matrix Size = %d\n", actualSize);

     for(row = 0; row < actualSize; row++){
  	sprintf(comma, "%s", "");
	for(column = 0; column < actualSize; column++){
           	  fprintf(fp, "%s%f",comma,matrixValue[row][column]);
//                  printf("\r %lf \r\n",matrixValue[row][column]);
                  sprintf(comma, "%s", ",");
        }
//        printf("\r\n");
        fprintf(fp, "\n");
    } 
}


Matrix  matrixMultiplication(Matrix A, Matrix B, Matrix C, int size){

    int i=0;
    int j=0;
    int k=0;

     for(i=0;i<size;i++){
     	for(j=0;j<size;j++){
     		for(k=0;k<size;k++){
                   C.matrixValue[i][j]+= (A.matrixValue[i][k] * B.matrixValue[k][j]);
                }
        }
     }

     return C;
}

int main(int argc, char*argv[]){

     int size=0;
     int row=0;
     Matrix A;
     Matrix B;
     Matrix C; 
     
     A.matrixValue=NULL;
     B.matrixValue=NULL;
     C.matrixValue=NULL;

     if(argc<2){
          perror("\nUsage ./matrixMultiplicationNaive.out <squareMatrixSize>\n");
          exit(-1);
      }
     size= atoi(argv[1]);  

    /*IF the Given Matirx Size is != 2^x , where x=2,3,4,5,....
      Increase the size to its nearest 2^x value *********** */

    A.matrixValue = malloc(size * sizeof(double *));
    memset(A.matrixValue,0,size*sizeof(double*));
    for(row=0;row<size;row++){
        A.matrixValue[row] = malloc(size*sizeof(double));
         memset(A.matrixValue[row],0,size*sizeof(double));
        if(A.matrixValue[row]==NULL){
            printf("\r\nMemory Allocation Failed at A");
        }
    }   

    B.matrixValue = malloc(size * sizeof(double *));
    memset(B.matrixValue,0,size*sizeof(double*));
    for(row=0;row<size;row++){
        B.matrixValue[row] = malloc(size*sizeof(double));
        memset(B.matrixValue[row],0,size*sizeof(double));
        if(B.matrixValue[row]==NULL){
            printf("\r\nMemory Allocation Failed at B");
        }
    }
   
    C.matrixValue = malloc(size * sizeof(double *));
    memset(C.matrixValue,0,size*sizeof(double*));
    for(row=0;row<size;row++){
        C.matrixValue[row] = malloc(size*sizeof(double));
        memset(C.matrixValue[row],0,size*sizeof(double));
        if(C.matrixValue[row]==NULL){
            printf("\r\nMemory Allocation Failed at C");
        }
    }   

    getMatrixInput(A.matrixValue,size);
    getMatrixInput(B.matrixValue,size);

    timing_start();   
    matrixMultiplication(A,B,C,size);
    timing_stop();

    print_timing();
    
    displayMatrix(C.matrixValue,size);
    
    for(row=0 ;row < size ;row++){
        free(A.matrixValue[row]);
        free(B.matrixValue[row]);
        free(C.matrixValue[row]);
    }
    free(A.matrixValue);
    free(B.matrixValue);
    free(C.matrixValue);
   return 0;
}
