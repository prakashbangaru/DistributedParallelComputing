#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <omp.h>
//#include "timing.h"
//#include "openmpi-x86_64/mpi.h"

#define INPUT_FILE_NAME "inputMatrix.txt"

//#define DEBUG(x)((flag==1)?(printf(x)):())


void readInputMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount);

int main(int argc, const char* argv[]){
    char **inputMatrix=NULL;
    char **resultMatrix=NULL;
    char **tempMatrix=NULL;
    int rowCount=0;
    int columnCount=0;
    int index=0;
    int numberOfIteration=0;
    int loopCount=0;
    int count =0;
    int lastRowIndex=0;
    int lastColumnIndex=0;
    int i=0;
    int j=0;

    rowCount = atoi(argv[1]);
    columnCount = atoi(argv[2]);
    numberOfIteration= atoi(argv[3]);
            
    lastRowIndex = rowCount-1;
    lastColumnIndex = columnCount-1; 
    inputMatrix = malloc(sizeof(char*)*rowCount);
    if(inputMatrix == NULL){
       printf("\r\nMemory Allocation Failed at inputMarix");
       exit(1);
    }
    memset(inputMatrix,0,sizeof(char*)*rowCount);


    resultMatrix = malloc(sizeof(char*)*rowCount);
    if(resultMatrix == NULL){
       printf("\r\nMemory Allocation Failed at resultMarix");
       exit(1);
    }
    memset(resultMatrix,0,sizeof(char*)*rowCount);

   for(index=0;index<rowCount;index++){
        inputMatrix[index] = malloc(columnCount*sizeof(char));
        resultMatrix[index] = malloc(columnCount*sizeof(char));
        memset(inputMatrix[index],0,columnCount*sizeof(char));
        memset(resultMatrix[index],0,columnCount*sizeof(char));
        if(inputMatrix[index]==NULL && resultMatrix[index]==NULL){
            printf("\r\nMemory Allocation Failed at inputMatrix && resultMatrix \n");
            exit(1);
        }
    } 

   readInputMatrix(INPUT_FILE_NAME,inputMatrix, rowCount, columnCount);

   printf("The Input Matrix\n");
  
   while(loopCount < numberOfIteration){
          
          printf("printing input Matrix \n");
          for(i=0;i<rowCount;i++){
            for(j=0;j<columnCount;j++){
                   printf("%c ",inputMatrix[i][j]);
            }
            printf("\n");
         } 

         /*Check for the Left Bottom Corner*/
          count =0;
         if(inputMatrix[lastRowIndex][1]=='1'){
           count++;
         }
         if(inputMatrix[lastRowIndex-1][0]=='1'){
           count++;
         }
         if(inputMatrix[lastRowIndex-1][1]=='1'){
           count++;
         }
         
         if(count > 1 && count <4){
            resultMatrix[lastRowIndex][0] = '1'; 
         }
         else{
            resultMatrix[lastRowIndex][0] = '0'; 
         }

         /*Check for the Right Bottom Corner*/
          count =0;
         if(inputMatrix[lastRowIndex][lastColumnIndex-1]=='1'){
           count++;
         }
         if(inputMatrix[lastRowIndex-1][lastColumnIndex]=='1'){
           count++;
         }
         if(inputMatrix[lastRowIndex-1][lastColumnIndex-1]=='1'){
           count++;
         }
         if(count > 1 && count <4){
            resultMatrix[lastRowIndex][lastColumnIndex] = '1'; 
         }
         else{
            resultMatrix[lastRowIndex][lastColumnIndex] = '0'; 
         }
           
         /*Check for the Right Top Corner*/
         count =0;
         if(inputMatrix[0][lastColumnIndex-1]=='1'){
           count++;
         }
         if(inputMatrix[1][lastColumnIndex] =='1'){
           count++;
         }
         if(inputMatrix[1][lastColumnIndex-1]=='1'){
           count++;
         }
         if(count > 1 && count <4){
            resultMatrix[0][lastColumnIndex] = '1'; 
         }
         else{
            resultMatrix[0][lastColumnIndex] = '0'; 
         }

         /*Check for the Left Top Corner*/
         count =0;
         if(inputMatrix[0][1]=='1'){
           count++;
         }
         if(inputMatrix[1][0]=='1'){
           count++;
         }
         if(inputMatrix[1][1]=='1'){
           count++;
         }
         if(count > 1 && count <4){
            resultMatrix[0][0] = '1'; 
         }
         else{
            resultMatrix[0][0] = '0'; 
         }

         /*check for the First Row*/
         for(j=1;j<columnCount;j++){
             count =0;
             if(inputMatrix[0][j-1]=='1'){
                count++;
             }
             if(inputMatrix[0][j+1]=='1'){
                count++;
             }
             if(inputMatrix[1][j]=='1'){
                count++;
             }
             if(inputMatrix[1][j-1]=='1'){
                count++;
             }
             if(inputMatrix[1][j+1]=='1'){
                count++;
             }

             if(count > 1 && count <4){
            	resultMatrix[0][j] = '1'; 
             }
             else{
            	resultMatrix[0][j] = '0'; 
            }
         }

         /*check for the Last Row*/
         for(j=1;j<columnCount;j++){
             count =0;
             if(inputMatrix[lastRowIndex][j-1]=='1'){
                count++;
             }
             if(inputMatrix[lastRowIndex][j+1]=='1'){
                count++;
             }
             if(inputMatrix[lastRowIndex-1][j]=='1'){
                count++;
             }
             if(inputMatrix[lastRowIndex-1][j-1]=='1'){
                count++;
             }
             if(inputMatrix[lastRowIndex-1][j+1]=='1'){
                count++;
             }

             if(count > 1 && count <4){
            	resultMatrix[lastRowIndex][j] = '1'; 
             }
             else{
            	resultMatrix[lastRowIndex][j] = '0'; 
            }
         }

         /*check for the First column*/
         for(i=1;i<lastRowIndex;i++){
             count =0;
             if(inputMatrix[i][1]=='1'){
                count++;
             }
             if(inputMatrix[i+1][0]=='1'){
                count++;
             }
             if(inputMatrix[i+1][1]=='1'){
                count++;
             }
             if(inputMatrix[i-1][0]=='1'){
                count++;
             }
             if(inputMatrix[i-1][1]=='1'){
                count++;
             }
             if(count > 1 && count <4){
            	resultMatrix[i][0] = '1'; 
             }
             else{
            	resultMatrix[i][0] = '0'; 
            }
         }

         /*check for the last column*/
         for(i=1;i<lastRowIndex;i++){
             count =0;
             if(inputMatrix[i][lastColumnIndex-1]=='1'){
                count++;
             }
             if(inputMatrix[i+1][lastColumnIndex]=='1'){
                count++;
             }
             if(inputMatrix[i+1][lastColumnIndex-1]=='1'){
                count++;
             }
             if(inputMatrix[i-1][lastColumnIndex]=='1'){
                count++;
             }
             if(inputMatrix[i-1][lastColumnIndex-1]=='1'){
                count++;
             }
             if(count > 1 && count <4){
            	resultMatrix[i][lastColumnIndex] = '1'; 
             }
             else{
            	resultMatrix[i][lastColumnIndex] = '0'; 
            }
        }

          /*Check for the Middle Matrix Value*/ 
          for(i=1;i<lastRowIndex;i++){
            for(j=1;j<lastColumnIndex;j++){
                 count =0;
	         if(inputMatrix[i][j-1]=='1'){
                      count++;
                 }		
	         if(inputMatrix[i+1][j-1]=='1'){
                      count++;
                 }		
	         if(inputMatrix[i-1][j-1]=='1'){
                      count++;
                 }		
	         if(inputMatrix[i][j+1]=='1'){
                      count++;
                 }		
	         if(inputMatrix[i+1][j+1]=='1'){
                      count++;
                 }		
	         if(inputMatrix[i-1][j+1]=='1'){
                      count++;
                 }		
	         if(inputMatrix[i+1][j]=='1'){
                      count++;
                 }		
	         if(inputMatrix[i-1][j]=='1'){
                      count++;
                 }		
                 if(count > 1 && count < 4){
            	      resultMatrix[i][j] = '1'; 
                 }
                 else{
            	      resultMatrix[i][j] = '0'; 
                 }
            }
         }
          printf("printing after Iteration\n");
          for(i=0;i<rowCount;i++){
            for(j=0;j<columnCount;j++){
                   printf("%c ",resultMatrix[i][j]);
            }
            printf("\n");
         } 

        tempMatrix = inputMatrix;
        inputMatrix = resultMatrix;
        resultMatrix = tempMatrix;
                    
        for(index=0;index<rowCount;index++){
            memset(tempMatrix[index],0,columnCount*sizeof(char));
        }
        //memset(tempMatrix,0,sizeof(char*)*rowCount);

        loopCount++;
        count=0;
   }

}


void readInputMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount){
   FILE *file = fopen(inputFileName, "r");
   int i=0;
   int j=0;
   char value = '\0';

   if (file){
      for ( i = 0; i< rowCount; i++ ){
    
         for ( j = 0; j < columnCount ; j++){
                
                if (!fscanf(file, "%c", &value)){ 
                    break; 
                }
                else{
                    if(value == ' ' || value == '\n'){
                      j--;
                      continue;
                    }
                    else{
                       inputMatrix[i][j] = value;
                    }
                }
         }
     }
            fclose(file);
  }
}

void displayMatrix(char **resultMatrix, int rowCount,int columnCount){
     int i=0;
     int j=0;
     FILE *fp=NULL;
     char comma[2] = {'\0','\0'};

     fp = fopen("outputMatrixFile.txt", "w");
     sprintf(comma, "%s", "");

     for(i = 0; i <rowCount; i++){
            for(j = 0; j <columnCount; j++){
               fprintf(fp, "%s%c",comma,resultMatrix[i][j]);
               sprintf(comma, "%s", " ");
            }
            fprintf(fp, "\n");
            sprintf(comma, "%s", "");
     }
    fprintf(fp, "\n");
    fclose(fp);
}


