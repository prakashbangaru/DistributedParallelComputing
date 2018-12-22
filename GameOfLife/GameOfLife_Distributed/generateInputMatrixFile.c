#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void getInputMatrixA(int rowSize,int columnSize);

int main(int argc, char *argv[]){
	
	int matrixSize=0;
	double** matrixA=NULL;
	double** matrixB=NULL;
	double** matrixC=NULL;
        int row =0;
        int column=0;
	
	if(argc <3){
		printf("generateMatirxInputFile.c <matrixSize>\r\n");
		exit(1);
	}
	
	row = atoi(argv[1]);	
       column = atoi(argv[2]);
	getInputMatrixA(row,column);
		
	return 0;
}

void getInputMatrixA(int rowSize,int columnSize){
  FILE *fp=NULL;
  int row=0;
  int column=0;
  char comma[2];
  double value=0.0;
  int x=0;
  struct drand48_data drandBuf; 
  fp = fopen("inputMatrix.txt", "w");

  for(row=0;row<rowSize;row++){
        sprintf(comma, "%s", "");
        for(column=0;column<columnSize;column++){
            drand48_r(&drandBuf, &value);
             x=(int)(value*2);
            fprintf(fp, "%s%d",comma,x);
            sprintf(comma, "%s", " ");
        }
        fprintf(fp, "\n");
   }
  fclose(fp);
}
