#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<string.h>
#include"timing.h"

#define MIN_MATRIX_SIZE 2

typedef struct{
int rowStart;
int rowEnd;
int columnStart;
int columnEnd;
double **matrixValue;
}Matrix;

double getRand(double min, double max){
    double d = (double)rand() / RAND_MAX;
    return min + d * (max - min);
}

void getMatrixInput(double **matrixValue,int size,int actualSize){
  FILE *fp=NULL;
  int row=0;
  int column=0;
  double min=0.00001;
  double  max=100000;
  char comma[2];
  double value=0.0;
 
  fp = fopen("inputMatrix.csv", "a");
  srand(time(NULL));
  fprintf(fp, "%d\n", actualSize);

  for(row=0;row<size;row++){
  	sprintf(comma, "%s", "");
    	for(column=0;column<size;column++){
     		if(row >= actualSize || column >= actualSize )
                	matrixValue[row][column]=0;
      		else{
//                        scanf("%lf",&value);
                        value= getRand(min,max);
                        matrixValue[row][column]=value; //BVP
           		fprintf(fp, "%s%f",comma,value);
//                        matrixValue[row][column]=2; //BVP
       		}
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


void matrixAddition(Matrix A, Matrix B, Matrix C){
        //BVP
        int ai=0,bi=0,ci=0;
        int aj=0,bj=0,cj=0;

        ci=C.rowStart;

      if(A.matrixValue != NULL && B.matrixValue != NULL){
   
      for(ai=A.rowStart,bi=B.rowStart; ai<=A.rowEnd && bi<=B.rowEnd ;ai++,bi++){
        cj=C.columnStart;
      	for(aj=A.columnStart,bj=B.columnStart; aj<=A.columnEnd && bj<=B.columnEnd ;aj++,bj++){
                  if( ci <= C.rowEnd && cj <= C.columnEnd){
                     C.matrixValue[ci][cj]=A.matrixValue[ai][aj]+B.matrixValue[bi][bj]; 
                  }
                  else
                       break;
                  cj++;
          }
                 ci++;
        }
 
      }
}


Matrix  matrixMultiplication(Matrix A, Matrix B, Matrix C, int size){
    
     Matrix A11; 
     Matrix A12; 
     Matrix A21; 
     Matrix A22; 
     Matrix B11; 
     Matrix B12;
     Matrix B21;
     Matrix B22;
     Matrix C11; 
     Matrix C12;
     Matrix C21;
     Matrix C22;
     Matrix D;
     Matrix D11; 
     Matrix D12;
     Matrix D21;
     Matrix D22;

     int halfSize = size/2;
     int halfSizeLastIndex = (size/2)-1;
     int lastIndex = size-1;
     int index=0,i=0,j=0;
    // Variables used for Strassen's Matrix Multiplication
     double P=0,Q=0,R=0,S=0,T=0,U=0,V=0; 

     
     if( size <= MIN_MATRIX_SIZE ){
        // Matrix Multiplication using Strassen's Method.

        P= (A.matrixValue[A.rowStart][A.columnStart] + A.matrixValue[A.rowEnd][A.columnEnd]) * (B.matrixValue[B.rowStart][B.columnStart] + B.matrixValue[B.rowEnd][B.columnEnd]);  
        Q= (A.matrixValue[A.rowEnd][A.columnStart] + A.matrixValue[A.rowEnd][A.columnEnd]) * (B.matrixValue[B.rowStart][B.columnStart]);
        R=  A.matrixValue[A.rowStart][A.columnStart] * ( B.matrixValue[B.rowStart][B.columnEnd] - B.matrixValue[B.rowEnd][B.columnEnd]);
        S=  A.matrixValue[A.rowEnd][A.columnEnd] * ( B.matrixValue[B.rowEnd][B.columnStart] - B.matrixValue[B.rowStart][B.columnStart]);
        T= (A.matrixValue[A.rowStart][A.columnStart] + A.matrixValue[A.rowStart][A.columnEnd]) * B.matrixValue[B.rowEnd][B.columnEnd];
        U= (A.matrixValue[A.rowEnd][A.columnStart] - A.matrixValue[A.rowStart][A.columnStart])* (B.matrixValue[B.rowStart][B.columnStart] + B.matrixValue[B.rowStart][B.columnEnd]);
        V=  (A.matrixValue[A.rowStart][A.columnEnd] - A.matrixValue[A.rowEnd][A.columnEnd]) * (B.matrixValue[B.rowEnd][B.columnStart] + B.matrixValue[B.rowEnd][B.columnEnd]);
        
        C.matrixValue[C.rowStart][C.columnStart]= P+S-T+V;
        C.matrixValue[C.rowStart][C.columnEnd]= R+T;
        C.matrixValue[C.rowEnd][C.columnStart]= Q+S;
        C.matrixValue[C.rowEnd][C.columnEnd]= P+R-Q+U;
     }
     else{
 	        D.matrixValue=NULL;
     		D.matrixValue = malloc(size * sizeof(double *)); 
     	//	Below Memset OPeration is a costly Operation 
     	//	memset(D.matrixValue,0,size*sizeof(double*));
     		for(index=0;index<size;index++){
       			D.matrixValue[index]==NULL;
                }

     		for(index=0;index<size;index++){
   			D.matrixValue[index] = malloc(size*sizeof(double));
     	//	Below Memset OPeration is a costly Operation
       	//	memset(D.matrixValue[index],0,size*sizeof(double));
       			if(D.matrixValue[index]==NULL){
                  		printf("\r\nMemory Allocation Failed at D");
                                exit(-1);
       			} 
     		}

                D.rowStart=0;
                D.rowEnd=size-1;
                D.columnStart=0;
                D.columnEnd=size-1;

         	A11.matrixValue = A12.matrixValue = A21.matrixValue = A22.matrixValue = A.matrixValue;
         	B11.matrixValue = B12.matrixValue = B21.matrixValue = B22.matrixValue = B.matrixValue;
         	C11.matrixValue = C12.matrixValue = C21.matrixValue = C22.matrixValue = C.matrixValue;
         	D11.matrixValue = D12.matrixValue = D21.matrixValue = D22.matrixValue = D.matrixValue;

         A11.rowStart=A.rowStart;                       A12.rowStart=A.rowStart;                 A21.rowStart=A.rowStart+halfSize; A22.rowStart=A.rowStart+halfSize;
         A11.rowEnd=A.rowStart+halfSizeLastIndex;       A12.rowEnd=A.rowStart+halfSizeLastIndex; A21.rowEnd=A.rowEnd;              A22.rowEnd=A.rowEnd; 
         A11.columnStart=A.columnStart;                    A12.columnStart=A.columnStart+halfSize;  A21.columnStart=A.columnStart;  A22.columnStart=A.columnStart+halfSize;
         A11.columnEnd=A.columnStart+halfSizeLastIndex; A12.columnEnd=A.columnEnd;           A21.columnEnd=A.columnStart+halfSizeLastIndex; A22.columnEnd=A.columnEnd; 

         B11.rowStart=B.rowStart;                       B12.rowStart=B.rowStart;                 B21.rowStart=B.rowStart+halfSize; B22.rowStart=B.rowStart+halfSize;
         B11.rowEnd=B.rowStart+halfSizeLastIndex;       B12.rowEnd=B.rowStart+halfSizeLastIndex; B21.rowEnd=B.rowEnd;              B22.rowEnd=B.rowEnd; 
         B11.columnStart=B.columnStart;                    B12.columnStart=B.columnStart+halfSize;  B21.columnStart=B.columnStart;  B22.columnStart=B.columnStart+halfSize;
         B11.columnEnd=B.columnStart+halfSizeLastIndex; B12.columnEnd=B.columnEnd;           B21.columnEnd=B.columnStart+halfSizeLastIndex; B22.columnEnd=B.columnEnd; 

         C11.rowStart=C.rowStart;                       C12.rowStart=C.rowStart;                 C21.rowStart=C.rowStart+halfSize; C22.rowStart=C.rowStart+halfSize;
         C11.rowEnd=C.rowStart+halfSizeLastIndex;       C12.rowEnd=C.rowStart+halfSizeLastIndex; C21.rowEnd=C.rowEnd;              C22.rowEnd=C.rowEnd; 
         C11.columnStart=C.columnStart;                    C12.columnStart=C.columnStart+halfSize;  C21.columnStart=C.columnStart;  C22.columnStart=C.columnStart+halfSize;
         C11.columnEnd=C.columnStart+halfSizeLastIndex; C12.columnEnd=C.columnEnd;           C21.columnEnd=C.columnStart+halfSizeLastIndex; C22.columnEnd=C.columnEnd; 
    
         D11.rowStart=D.rowStart;                       D12.rowStart=D.rowStart;                 D21.rowStart=D.rowStart+halfSize; D22.rowStart=D.rowStart+halfSize;
         D11.rowEnd=D.rowStart+halfSizeLastIndex;       D12.rowEnd=D.rowStart+halfSizeLastIndex; D21.rowEnd=D.rowEnd;              D22.rowEnd=D.rowEnd; 
         D11.columnStart=D.columnStart;                    D12.columnStart=D.columnStart+halfSize;  D21.columnStart=D.columnStart;  D22.columnStart=D.columnStart+halfSize;
         D11.columnEnd=D.columnStart+halfSizeLastIndex; D12.columnEnd=D.columnEnd;           D21.columnEnd=D.columnStart+halfSizeLastIndex; D22.columnEnd=D.columnEnd; 
       
   
         matrixAddition(matrixMultiplication(A11,B11,C11,halfSize),matrixMultiplication(A12,B21,D11,halfSize),C11);
         matrixAddition(matrixMultiplication(A11,B12,C12,halfSize),matrixMultiplication(A12,B22,D12,halfSize),C12);
         matrixAddition(matrixMultiplication(A21,B11,C21,halfSize),matrixMultiplication(A22,B21,D21,halfSize),C21);
         matrixAddition(matrixMultiplication(A21,B12,C22,halfSize),matrixMultiplication(A22,B22,D22,halfSize),C22);
     
     	 for(index=0;index<size;index++){
               if(D.matrixValue[index]!=NULL)
   	 	free(D.matrixValue[index]);
     	 }

          if(D.matrixValue!=NULL)
   	   free(D.matrixValue);
   }
     return C;
}

int main(int argc, char*argv[]){

     int size=0;
     int row=0;
     int actualSize=0;
     int exponentValue=0;
     Matrix A;
     Matrix B;
     Matrix C; 
     
     A.matrixValue=NULL;
     B.matrixValue=NULL;
     C.matrixValue=NULL;
 
     A.rowStart=0;
     A.columnStart=0;
     B.rowStart=0;
     B.columnStart=0;
     C.rowStart=0;
     C.columnStart=0;

     if(argc<2){
          perror("\nUsage ./matrixMultiplicationOptimized.out <squareMatrixSize>\n");
          exit(-1);
      }
     size= atoi(argv[1]); 
   //  printf("Enter Size of matrix\n");   
   //  scanf("%d",&size);
     actualSize = size;
    

    /*IF the Given Matirx Size is != 2^x , where x=2,3,4,5,....
      Increase the size to its nearest 2^x value *********** */
    if(size & size-1 != 0){
        exponentValue = log(size)/log(2);
        size = pow(2,exponentValue+1);
    }
    
     A.rowEnd=size-1;
     A.columnEnd=size-1;
     B.rowEnd=size-1;
     B.columnEnd=size-1;
     C.rowEnd=size-1;
     C.columnEnd=size-1;
     

    A.matrixValue = malloc(size * sizeof(double *));
    if(A.matrixValue==NULL){
            printf("\r\nMemory Allocation Failed at A1");
            exit(-1);
    }
    memset(A.matrixValue,0,size*sizeof(double*));
    for(row=0;row<size;row++){
        A.matrixValue[row] = malloc(size*sizeof(double));
         memset(A.matrixValue[row],0,size*sizeof(double));
        if(A.matrixValue[row]==NULL){
            printf("\r\nMemory Allocation Failed at A");
            exit(-1);
        }
    }   


    B.matrixValue = malloc(size * sizeof(double *));
    if(B.matrixValue==NULL){
            printf("\r\nMemory Allocation Failed at B1");
            exit(-1);
    }
    memset(B.matrixValue,0,size*sizeof(double*));
    for(row=0;row<size;row++){
        B.matrixValue[row] = malloc(size*sizeof(double));
        memset(B.matrixValue[row],0,size*sizeof(double));
        if(B.matrixValue[row]==NULL){
            printf("\r\nMemory Allocation Failed at B");
            exit(-1);
        }
    }
   
    C.matrixValue = malloc(size * sizeof(double *));
    if(C.matrixValue==NULL){
            printf("\r\nMemory Allocation Failed at C1");
            exit(-1);
    }
    memset(C.matrixValue,0,size*sizeof(double*));
    for(row=0;row<size;row++){
        C.matrixValue[row] = malloc(size*sizeof(double));
        memset(C.matrixValue[row],0,size*sizeof(double));
        if(C.matrixValue[row]==NULL){
            printf("\r\nMemory Allocation Failed at C");
            exit(-1);
        }
    }   

    getMatrixInput(A.matrixValue,size,actualSize);
    getMatrixInput(B.matrixValue,size,actualSize);

    timing_start();   
    matrixMultiplication(A,B,C,size);
    timing_stop();

    print_timing();
    
    displayMatrix(C.matrixValue,actualSize);
    
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
