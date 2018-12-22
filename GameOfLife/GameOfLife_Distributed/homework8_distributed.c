#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include <stdbool.h>
#include "timing.h"
#include "openmpi-x86_64/mpi.h"
#define INPUT_FILE_NAME "inputMatrix.txt"
#define debugPrintf(fmt, ...) \
            do { if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); } while (0)


void readInputMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount);
void readFirstTwoRowMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount);
void readLastTwoRowMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount);
void readFirstTwoColumnMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount);
void readLastTwoColumnMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount);
void writeUpdatedMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount);
void printMatrix(char **matrix, int rowCount, int  columnCount);

int main(int argc, char* argv[]){
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
    int rank=0;
    int numberOfProcess=0;
    int root=0;


   if(argc <4){
        printf("./part_8_parallel.out <rowCount> <columnCount> <numberOfIteration>\n");
        exit(-1);
    }


    rowCount = atoi(argv[1]);
    columnCount = atoi(argv[2]);
    numberOfIteration= atoi(argv[3]);


    char firstColumn[rowCount];
    char lastColumn[rowCount]; 
    char lastTwoColumnMatrix[rowCount*2];
    char firstRow[columnCount+1]; 
    char lastRow[columnCount+1];
    char lastBeforeRow[columnCount];
    int broadcastMessage=0;

    memset(lastTwoColumnMatrix,0,sizeof(char)*2*rowCount);
    memset(firstRow,0,sizeof(char)*columnCount+1);
    memset(lastRow,0,sizeof(char)*columnCount+1);
    memset(firstColumn,0,sizeof(char)*rowCount);
    memset(lastColumn,0,sizeof(char)*rowCount);

    lastRowIndex = rowCount-1;
    lastColumnIndex = columnCount-1;


    timing_start();
    MPI_Status status;
    MPI_Init(&argc,&argv);
    MPI_Comm_rank(MPI_COMM_WORLD,&rank);
    MPI_Comm_size(MPI_COMM_WORLD,&numberOfProcess);

   if(rank == 3 || rank == 4 || rank == 0){
    inputMatrix = malloc(sizeof(char*)*rowCount);
    if(inputMatrix == NULL){
       printf("\r\nMemory Allocation Failed at inputMarix");
       exit(1);
    }
    memset(inputMatrix,0,sizeof(char*)*rowCount);
    for(index=0;index<rowCount;index++){
        inputMatrix[index] = malloc(columnCount*sizeof(char));
        memset(inputMatrix[index],0,columnCount*sizeof(char));
        if(inputMatrix[index]==NULL && resultMatrix[index]==NULL){
            printf("\r\nMemory Allocation Failed at inputMatrix && resultMatrix \n");
            exit(1);
        }
    } 
  }
  if(rank == 1){
    char **firstTwoRow=NULL; 

    firstTwoRow = malloc(sizeof(char*)*2);
    firstTwoRow[0] = malloc(sizeof(char)*columnCount);
    firstTwoRow[1] = malloc(sizeof(char)*columnCount);

    memset(firstTwoRow[0],0,sizeof(char)*columnCount);
    memset(firstTwoRow[1],0,sizeof(char)*columnCount);

    while(true){
    readFirstTwoRowMatrix(INPUT_FILE_NAME,firstTwoRow, rowCount, columnCount);
         /*check for the First Row*/
         for(j=1;j<(columnCount-1);j++){
             count =0;
             if(firstTwoRow[0][j-1]=='1'){
                count++;
             }
             if(firstTwoRow[0][j+1]=='1'){
                count++;
             }
             if(firstTwoRow[1][j]=='1'){
                count++;
             }
             if(firstTwoRow[1][j-1]=='1'){
                count++;
             }
             if(firstTwoRow[1][j+1]=='1'){
                count++;
             }

             if(count > 1 && count <4){
            	firstRow[j] = '1';
             }
             else{
            	firstRow[j] = '0'; 
            }
         }
    if(DEBUG == 1){
    /*Send Updated First Row to the Master*/ 
    MPI_Send(&firstRow[1],columnCount-2, MPI_BYTE, 0, 1, MPI_COMM_WORLD);
    }
    /*Recieve Broadcast Message from the Process 0*/
     MPI_Barrier(MPI_COMM_WORLD);
     MPI_Bcast(&broadcastMessage,1, MPI_INT, root, MPI_COMM_WORLD);
  
    if(broadcastMessage == 1){
       continue;
    }
    else{
       break;
    }
   
   } 

   free(firstTwoRow[0]);
   free(firstTwoRow[1]);
   free(firstTwoRow);
  }

  if(rank == 2){
    char **lastTwoRow=NULL; 

    lastTwoRow = malloc(sizeof(char*)*2);

    lastTwoRow[0] = malloc(sizeof(char)*columnCount);
    lastTwoRow[1] = malloc(sizeof(char)*columnCount);

    memset(lastTwoRow[0],0,sizeof(char)*columnCount);
    memset(lastTwoRow[1],0,sizeof(char)*columnCount);

    while(true){
    readLastTwoRowMatrix(INPUT_FILE_NAME,lastTwoRow, rowCount, columnCount);
         /*check for the Last Row*/
         for(j=1;j<(columnCount-1);j++){
             count =0;
             if(lastTwoRow[1][j-1]=='1'){
                count++;
             }
             if(lastTwoRow[1][j+1]=='1'){
                count++;
             }
             if(lastTwoRow[0][j]=='1'){
                count++;
             }
             if(lastTwoRow[0][j-1]=='1'){
                count++;
             }
             if(lastTwoRow[0][j+1]=='1'){
                count++;
             }

             if(count > 1 && count <4){
            	lastRow[j] = '1'; 
             }
             else{
            	lastRow[j] = '0'; 
            }
         }
    if(DEBUG == 1){
     /*Send Updated Last Row to the Master*/ 
     MPI_Send(&lastRow[1],columnCount-2, MPI_BYTE, 0, 2, MPI_COMM_WORLD);
    } 
    /*Recieve Broadcast Message from the Process 0*/
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Bcast(&broadcastMessage,1, MPI_INT, root, MPI_COMM_WORLD);
    if(broadcastMessage == 1){
       continue;
    }
    else{
       break;
    }   
   }

    free(lastTwoRow[0]);
    free(lastTwoRow[1]);
    free(lastTwoRow);
  }
  if(rank == 3){
    char **firstTwoColumnMatrix=NULL; 
 
    firstTwoColumnMatrix = malloc(sizeof(char*)*rowCount);
    for(index=0;index < rowCount; index++){    
  	  firstTwoColumnMatrix[index] = malloc(sizeof(char)*2);
          memset(firstTwoColumnMatrix[index],0,sizeof(char)*2);
    }
    while(true){

    readFirstTwoColumnMatrix(INPUT_FILE_NAME, firstTwoColumnMatrix,rowCount,columnCount);
   
     /*check for the First column*/
         for(i=1;i<lastRowIndex;i++){
             count =0;
             if(firstTwoColumnMatrix[i][1]=='1'){
                count++;
             }
             if(firstTwoColumnMatrix[i+1][0]=='1'){
                count++;
             }
             if(firstTwoColumnMatrix[i+1][1]=='1'){
                count++;
             }
             if(firstTwoColumnMatrix[i-1][0]=='1'){
                count++;
             }
             if(firstTwoColumnMatrix[i-1][1]=='1'){
                count++;
             }
             if(count > 1 && count <4){
            	firstColumn[i] = '1'; 
             }
             else{
            	firstColumn[i] = '0'; 
            }
         }

    if(DEBUG == 1){
      /*Send Updated first Column to the Master*/ 
       MPI_Send(&firstColumn[1],rowCount-2, MPI_BYTE, 0, 3, MPI_COMM_WORLD);
     }
    /*Recieve Broadcast Message from the Process 0*/
     MPI_Barrier(MPI_COMM_WORLD);
     MPI_Bcast(&broadcastMessage,1, MPI_INT, root, MPI_COMM_WORLD);
 
    if(broadcastMessage == 1){
       continue;
    }
    else{
       break;
    }
   }
    
    for(index=0;index < rowCount; index++){    
  	  free(firstTwoColumnMatrix[index]);
    }
    free(firstTwoColumnMatrix);
  }

  if(rank == 4){
    char **lastTwoColumnMatrix=NULL; 
 
    lastTwoColumnMatrix = malloc(sizeof(char*)*rowCount);
    for(index=0;index < rowCount; index++){    
  	  lastTwoColumnMatrix[index] = malloc(sizeof(char)*2);
          memset(lastTwoColumnMatrix[index],0,sizeof(char)*2);
    }
    while(true){
    readLastTwoColumnMatrix(INPUT_FILE_NAME, lastTwoColumnMatrix,rowCount,columnCount);
   
         /*check for the last column*/
         for(i=1;i<lastRowIndex;i++){
             count =0;
             if(lastTwoColumnMatrix[i][0]=='1'){
                count++;
             }
             if(lastTwoColumnMatrix[i+1][1]=='1'){
                count++;
             }
             if(lastTwoColumnMatrix[i+1][0]=='1'){
                count++;
             }
             if(lastTwoColumnMatrix[i-1][1]=='1'){
                count++;
             }
             if(lastTwoColumnMatrix[i-1][0]=='1'){
                count++;
             }
             if(count > 1 && count <4){
            	lastColumn[i] = '1'; 
             }
             else{
            	lastColumn[i] = '0'; 
            }
        }

    if(DEBUG == 1){
    /*Send Updated Last Column to the Master*/ 
    MPI_Send(&lastColumn[1],rowCount-2, MPI_BYTE, 0, 4, MPI_COMM_WORLD);
    }     
    /*Recieve Broadcast Message from the Process 0*/
    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(&broadcastMessage,1, MPI_INT, root, MPI_COMM_WORLD);
 
    if(broadcastMessage == 1){
       continue;
    }
    else{
       break;
    }
   }

    for(index=0;index < rowCount; index++){    
  	  free(lastTwoColumnMatrix[index]);
    }
    free(lastTwoColumnMatrix);
  }

  if(rank == 3 || rank == 4){ 
    for(index=0;index<rowCount;index++){
        free(inputMatrix[index]);
    }
    free(inputMatrix); 
  }
  

  if(rank == 0){ 

    resultMatrix = malloc(sizeof(char*)*rowCount);
    if(resultMatrix == NULL){
       printf("\r\nMemory Allocation Failed at resultMarix");
       exit(1);
    }
    memset(resultMatrix,0,sizeof(char*)*rowCount);

   for(index=0;index<rowCount;index++){
        resultMatrix[index] = malloc(columnCount*sizeof(char));
        memset(resultMatrix[index],0,columnCount*sizeof(char));
        if( resultMatrix[index]==NULL){
            printf("\r\nMemory Allocation Failed at resultMatrix \n");
            exit(1);
        }
    } 

   readInputMatrix(INPUT_FILE_NAME,inputMatrix, rowCount, columnCount);

   printf("Start\n"); 
   printMatrix(inputMatrix,rowCount, columnCount);

   while(loopCount <= numberOfIteration){
          
          loopCount++;
           
         /*Check for the Left Bottom Corner*/
          count =0;
         if(inputMatrix[lastRowIndex][1]== '1'){
           count++;
         }
         if(inputMatrix[lastRowIndex-1][0]== '1'){
           count++;
         }
         if(inputMatrix[lastRowIndex-1][1]== '1'){
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

         /*Recieve First Row from the Process 1*/
         MPI_Recv(&resultMatrix[0][1],columnCount-2, MPI_BYTE, 1, 1, MPI_COMM_WORLD, &status); 
            
          
         /*Recieve Last Row from the Process 2*/
         MPI_Recv(&resultMatrix[lastRowIndex][1],columnCount-2, MPI_BYTE, 2, 2, MPI_COMM_WORLD, &status); 
        
         /*Recieve First Column from the Process 3*/
         MPI_Recv(&firstColumn[1],rowCount-2, MPI_BYTE, 3, 3, MPI_COMM_WORLD, &status); 
         updateFirstColumn(resultMatrix, firstColumn,rowCount);
 
         /*Recieve First Column from the Process 3*/
         MPI_Recv(&lastColumn[1],rowCount-2, MPI_BYTE, 4, 4, MPI_COMM_WORLD, &status); 
         updateLastColumn(resultMatrix,lastColumn,rowCount,columnCount);
         
         /*Write the UPdated Matrix into the File*/  
         writeUpdatedMatrix("inputMatrix.txt", resultMatrix,rowCount, columnCount);
          
          if(loopCount == numberOfIteration){
             broadcastMessage = -1;
             MPI_Bcast(&broadcastMessage,1,MPI_INT,root,MPI_COMM_WORLD);
             MPI_Barrier(MPI_COMM_WORLD);

              printf("\n Round %d\n",loopCount);
              printMatrix(resultMatrix,rowCount, columnCount);
             break;
          }

          if(loopCount < numberOfIteration){
          broadcastMessage = 1; 
          MPI_Bcast(&broadcastMessage,1,MPI_INT,root,MPI_COMM_WORLD);
          MPI_Barrier(MPI_COMM_WORLD);
          }

          printf("\n Round %d\n",loopCount);
          printMatrix(resultMatrix,rowCount, columnCount);

        tempMatrix = inputMatrix;
        inputMatrix = resultMatrix;
        resultMatrix = tempMatrix;
                    
        for(index=0;index<rowCount;index++){
            memset(tempMatrix[index],0,columnCount*sizeof(char));
        }

        count=0;
    }
        
       for(index=0;index<rowCount;index++){
          free(inputMatrix[index]);
          free(resultMatrix[index]);
       }
       free(inputMatrix);
       free(resultMatrix);
 
   timing_stop();
   print_timing();
  }
  MPI_Finalize();
  return 0;
}

void printMatrix(char **matrix, int rowCount, int columnCount){
     int i =0;
    int j =0;

    for(i=0;i<rowCount;i++){
        for(j=0;j<columnCount;j++){
            printf("%c ",matrix[i][j]);
        }
        printf("\n");
    } 
}

void writeUpdatedMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount){
   FILE *file = fopen(inputFileName, "w");
   int i=0;
   int j=0;
   char value = '\0';

   if (file){
      for ( i = 0; i< rowCount; i++ ){
         for ( j = 0; j < columnCount ; j++){
                  fputc(inputMatrix[i][j],file); 
                  fputc(' ',file); 
                }
                fputc('\n',file); 
         }
     }
            fclose(file);
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

void readFirstTwoRowMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount){
   FILE *file = fopen(inputFileName, "r");
   int i=0;
   int j=0;
   char value = '\0';

   if (file){
      for ( i = 0; i<2; i++ ){
    
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


void readLastTwoRowMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount){
   FILE *file = fopen(inputFileName, "r");
   int i=0;
   int j=0;
   char value = '\0';
   int skipCount=0;
   int rowIndex=0;
   int buffer[BUFSIZ];

   if (file){
     
     while(i < (rowCount-2)){
           if(fgets(buffer,BUFSIZ,file)){
             i++;
           }
      }

      for ( i = 0; i < 2; i++ ){
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

void readFirstTwoColumnMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount){
   FILE *file = fopen(inputFileName, "r");
   int i=0;
   int j=0;
   char value = '\0';

   if (file){
      for ( i = 0; i<rowCount; i++ ){
    
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
                       if(j<2){
                         inputMatrix[i][j] = value;
                       }
                    }
                }
         }
     }
     fclose(file);
  }
}

void readLastTwoColumnMatrix(char *inputFileName, char **inputMatrix, int rowCount, int columnCount){
   FILE *file = fopen(inputFileName, "r");
   int i=0;
   int j=0;
   char value = '\0';
   int columnIndex=0;

   if (file){
      for ( i = 0; i<rowCount; i++ ){
         columnIndex =0;
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
                       if( j > (columnCount-3)){
                          inputMatrix[i][columnIndex] = value;
                          columnIndex++;
                       }
                    }
                }
         }
     }
            fclose(file);
  }
}



int updateFirstColumn(char** inputMatrix, char* updatedFirstColumnMatrix, int rowCount){
      int i =0;
      for(i=1;i<rowCount-1;i++){
        inputMatrix[i][0]= updatedFirstColumnMatrix[i];
      }
}


int updateLastColumn(char** inputMatrix, char* updatedLastColumnMatrix, int rowCount, int columnCount){
      int i =0;
      int lastColumnIndex = columnCount-1;
    
      for(i=1;i<rowCount-1;i++){
        inputMatrix[i][lastColumnIndex]= updatedLastColumnMatrix[i];
      }
}
