#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void getInputMatrixA(int size);
void getInputMatrixB(int size);

int main(int argc, char *argv[]){
	
	int matrixSize=0;
	double** matrixA=NULL;
	double** matrixB=NULL;
	double** matrixC=NULL;
        int row =0;
	
	if(argc <2){
		printf("generateMatirxInputFile.c <matrixSize>\r\n");
		exit(1);
	}
	
	matrixSize = atoi(argv[1]);	
	getInputMatrixA(matrixSize);
	getInputMatrixB(matrixSize);
		
	return 0;
}

void getInputMatrixA(int size){
  FILE *fp=NULL;
  int row=0;
  int column=0;
  char comma[2];
  double value=0.0;
  struct drand48_data drandBuf; 
  fp = fopen("inputMatrixA.txt", "w");

  for(row=0;row<size;row++){
        sprintf(comma, "%s", "");
        for(column=0;column<size;column++){
            drand48_r(&drandBuf, &value);
            //matrixValue[row][column]=value;
            fprintf(fp, "%s%f",comma,value);
            //sprintf(comma, "%s", ",");
            sprintf(comma, "%s", " ");
        }
        fprintf(fp, "\n");
   }
  fclose(fp);
}

void getInputMatrixB(int size){
  FILE *fp=NULL;
  int row=0;
  int column=0;
  char comma[2];
  double value=0.0;
  struct drand48_data drandBuf; 
  fp = fopen("inputMatrixB.txt", "w");

  for(row=0;row<size;row++){
        sprintf(comma, "%s", "");
        for(column=0;column<size;column++){
            drand48_r(&drandBuf, &value);
            //matrixValue[row][column]=value;
            fprintf(fp, "%s%f",comma,value);
            //sprintf(comma, "%s", ",");
            sprintf(comma, "%s", " ");
        }
        fprintf(fp, "\n");
   }
  fclose(fp);
}
