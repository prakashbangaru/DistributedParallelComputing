#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX_NUMBER_COUNT 11

typedef struct{
 int rowStart;
 int rowEnd;
 int numberOfRows;
// int distributionCount[MAX_NUMBER_COUNT];
 int *distributionCount;
 int **matrix;
 int matrixSize;
}strips;


void calculateDistributionCount(strips *arg){
     // strips *stripInfo = (strips *) arg;
      strips* stripInfo = arg;
      int row = 0;
      int column=0;
      int matrixValue=0;
      
      for(row = stripInfo->rowStart; row <= stripInfo->rowEnd; row++){
          for(column = 0; column < stripInfo->matrixSize; column++){
                matrixValue= stripInfo->matrix[row][column];
                stripInfo->distributionCount[matrixValue]++;
          }
      }
}

double getRand(double min, double max){
    double d = (double)rand() / RAND_MAX;
    return min + d * (max - min);
}

void getMatrixInput(int **matrix, int size){
     int row=0;
     int column=0;
     int matrixValue=0;
     double min=0.00001;
     double max=10.0000;
     FILE *fp=NULL;
     char comma[2];

     fp = fopen("inputMatrix.csv", "a");

     if(fp==NULL){
       printf("\ninputMatrix.csv File not found\n");
       exit(-1);
     }

     for(row = 0; row < size;row++){
       sprintf(comma, "%s", "");
       for(column = 0;column < size;column++){
         matrixValue = getRand(min,max);
         matrixValue = (matrixValue%10)+ 1;
         matrix[row][column]=matrixValue;
         fprintf(fp, "%s%d",comma,matrixValue);
         sprintf(comma, "%s", ",");
       }
       fprintf(fp,"\n");
     }
   fclose(fp);
   fp = NULL;
}

int main(int argc, char const* argv[]){
    timing_start();
   
    int matrixSize=0;
    int numberOfThread=0;
    int index=0;
    int **matrix=NULL;
    int stripRowSize=0;
    int remainingRows=0;
    int distributionValue[MAX_NUMBER_COUNT];
    int create_error=0;
    int join_error=0;
    int i=0;
    int row=0;
    int column=0; 
 
    if (argc < 3){
	printf("./DistributionValueSerial.out <matrix Size> <P must be 1)>\n");
	exit(-1);
     }

   matrixSize=atoi(argv[1]);
   numberOfThread = atoi(argv[2]);

   if ( numberOfThread != 1){
        printf("./DistributionValueSerial.out <matrix Size> <P must be 1>\n");
        exit(-1);
     }

   strips  stripInfo[numberOfThread];

   memset(distributionValue,0,MAX_NUMBER_COUNT*sizeof(int));

   if(matrixSize > 1){
     matrix = malloc(matrixSize * sizeof(int *));
     if(matrix==NULL){
            printf("\r\nMemory Allocation Failed");
            exit(-1);
    }
    memset(matrix,0,matrixSize*sizeof(int*));

    for(index=0;index<matrixSize;index++){
        matrix[index] = malloc(matrixSize*sizeof(double));
        if(matrix[index]==NULL){
            printf("\r\nMemory Allocation Failed");
            exit(-1);
        }
        memset(matrix[index],0,matrixSize*sizeof(double));
    }

   /*Get the Matrix Input*/
    getMatrixInput(matrix,matrixSize);
   
   /*Assiginig Number of Rows to each Worker*/
    stripRowSize= matrixSize/numberOfThread;
    remainingRows = matrixSize % numberOfThread;

    for(index=0; index < numberOfThread ;index++){
      stripInfo[index].numberOfRows = stripRowSize;    
     if(remainingRows){
        stripInfo[index].numberOfRows++;
        --remainingRows;
     }
     stripInfo[index].distributionCount = malloc(MAX_NUMBER_COUNT*sizeof(int));
     if( stripInfo[index].distributionCount == NULL ){
            printf("\r\nMemory Allocation Failed at 1");
            exit(-1);
     }
     memset(stripInfo[index].distributionCount,0,MAX_NUMBER_COUNT*sizeof(int));
     stripInfo[index].matrix = matrix;
     stripInfo[index].matrixSize=matrixSize;
    }
   
  /*Assigning Row Boundary Value to each strips*/ 
   stripInfo[0].rowStart = 0;
   stripInfo[0].rowEnd = stripInfo[0].numberOfRows-1;

   for(index=1; index < numberOfThread ; index++){
     stripInfo[index].rowStart = stripInfo[index-1].rowEnd + 1;
     stripInfo[index].rowEnd = stripInfo[index-1].rowEnd + stripInfo[index].numberOfRows;
   }
  
   /*Calculate the Distribution Count*/
   for (index = 0; index < numberOfThread; ++index){
       calculateDistributionCount(&stripInfo[index]);
       for(i = 1;i < MAX_NUMBER_COUNT;i++){
        distributionValue[i] += stripInfo[index].distributionCount[i];      
      }
   }
   
   for (index = 0; index < numberOfThread; ++index){
      free(stripInfo[index].distributionCount);
   }
  }
  else if(matrixSize==1){
      distributionValue[matrix[0][0]]++;
  }

   for(i=1;i< MAX_NUMBER_COUNT;i++){
      printf("\r\n %d Count = %d\r\n",i,distributionValue[i]);
   }
     timing_stop();
     print_timing();
    // pthread_exit(NULL);
}
