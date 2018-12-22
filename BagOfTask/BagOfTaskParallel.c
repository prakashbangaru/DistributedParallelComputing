#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<stdbool.h>
#include"timing.h"

#define WORD_LENGTH 25
#define LINE_LENGTH 50
#define NUMBER_OF_TASKS 26
#define ASCII_VALUE_SMALL_A 97
#define ASCII_VALUE_SMALL_Z 122
#define MAX_NUMBER_OF_TASKS 26
#define UNIQUE_WORD_LENGTH 6

typedef struct{
char **wordList;
long *wordLength;
long *taskWordIndex;
long *uniqueWordIndex;
long sixLetterCount;//number of Words with Length >=6
long uniqueWordCount;
long wordCount;
}Tasks;

typedef struct{
Tasks *task;
int startTaskIndex;
int numberOfTasks;
long uniqueWordCount;
}Worker;


/*If the Word is Unique hen return word length else zero*/
long getUniqueWordLength(char *word){
 bool flag[26];
 long i=0;
 int letter=0;
 int index=0;

  memset(flag,false,sizeof(bool)*26);

 for(i=0; (word[i]!='\n')&&(word[i]!='\r')&&(word[i]!='\0');i++){
   letter = (int)word[i];
   index = letter - ASCII_VALUE_SMALL_A;
   if(letter >=ASCII_VALUE_SMALL_A &&
      letter <= ASCII_VALUE_SMALL_Z){
   if(flag[index]==false)
      flag[index]=true;
   else
      return 0;
  }
  else
    return 0;
 }
  return i;
}

long taskOperation(Tasks *task,int taskIndex){
    long i=0;
//    long wordCount = task->wordCount;
    char *word=NULL;
    long wordLength=0;
    long wordIndex=0;

 //   printf(" Task =%d Word =%s",taskIndex,task->wordList[task->taskWordIndex[0]]);
    for(i=0;i< task->wordCount;i++){
       wordIndex = task->taskWordIndex[i];
       word = task->wordList[wordIndex];
       wordLength = getUniqueWordLength(word);
       task->wordLength[wordIndex] = wordLength;
       if(wordLength!=0){
          task->uniqueWordCount++;
          if(wordLength>=UNIQUE_WORD_LENGTH){
             task->uniqueWordIndex[task->sixLetterCount]=wordIndex;
             task->sixLetterCount++;
          }
       }
   }

   return task->uniqueWordCount;
} 

void workerProcess_run(void *paramWorker){
     Worker *worker= (Worker*)paramWorker;
     int i =0;
     long uniqueWordCount=0;
     int lastTaskIndex= worker->startTaskIndex +worker->numberOfTasks-1;
     for(i=worker->startTaskIndex; i<=lastTaskIndex; i++){
            worker->uniqueWordCount += taskOperation(&(worker->task[i]),i);
     }
}

long getTotalWordCountAndUpdateTaskWordCount(FILE *fp,char** fileContent,Tasks *task,\
                                         long numberOfLines,long** taskWordIndexArray){

  long index=0;
  int lineSize = sizeof(char)*LINE_LENGTH;
  int firstIndexValue =0;
  int taskIndex=0;
  long taskWordIndex[NUMBER_OF_TASKS];
  long tempIndex;
  int numberOfTasksForWorker=0;

   memset(taskWordIndex,0,sizeof(long)*NUMBER_OF_TASKS);
  taskIndex=0;

  while((index < numberOfLines) && (fgets(fileContent[index], lineSize, fp) != NULL)){ //BVP
    /*Get the First letter of the Word and Convert it to its ASCII value*leContentAndUpdateTaskWordCount*/
    firstIndexValue = (int)fileContent[index][0];
    /*Check the First Letter Start with Small letter a to z*/
    if((firstIndexValue >= ASCII_VALUE_SMALL_A) &&
       (firstIndexValue <= ASCII_VALUE_SMALL_Z)){
    /*word start with 'a' assigned to task 0 and word with 'b' assigned to task 1 and so on*/
     taskIndex = firstIndexValue - ASCII_VALUE_SMALL_A;
     if((taskIndex < NUMBER_OF_TASKS) && (taskIndex >=0)){
        tempIndex=taskWordIndex[taskIndex];
        taskWordIndexArray[taskIndex][tempIndex]=index;
        taskWordIndex[taskIndex]++;
        task[taskIndex].wordCount++;
        index++;      
     }
     else{
         /*Reading only the words starts with small alphabets*/
        memset(fileContent[index],0,lineSize);
     }
    }
    else{
     /*Reading only the words starts with small alphabets*/
     memset(fileContent[index],0,lineSize);
    }
  }

  return index;

}

long getNumberOfLines(FILE *fp){
  char line[LINE_LENGTH];
  long counter=0;
  
  while(fgets(line, sizeof(line), fp) != NULL){
        counter++;
  }
  rewind(fp);
  return counter;
}


void writeToOutPutFile(FILE* outPutFilePtr,long **taskUniqueWordIndex,\
                       char **fileContent,Tasks *task){
   int taskIndex=0;
   long wordIndex=0;
   long i =0;
  for(taskIndex=0;taskIndex<NUMBER_OF_TASKS;taskIndex++){
    for(i=0;i<task[taskIndex].sixLetterCount;i++){
     wordIndex=taskUniqueWordIndex[taskIndex][i];
     fprintf(outPutFilePtr,"%s",fileContent[wordIndex]);
   }
  }
}

 void sample(){
}
int main(int argc, char *argv[]){

  char **fileContent=NULL;
  long fileSize=0;
  FILE *fp=NULL;
  long numberOfLines=0;
  long index=0;
  long *wordLength=NULL;
  long **taskUniqueWordIndex=NULL;
  long **taskWordIndex=NULL;
  int numberOfThreads=0;
  Tasks *task;
  long totalWordCount=0;
  int taskIndex=0;
  int minNumberOfTaskPerProcess=0;  
  FILE *outPutFilePtr=NULL;
  int workerIndex=0;
  int remainingProcess =0;
  long totalUniqueWordCount=0;
  
  if(argc < 3){
    printf("./bagOfTask <FileName> <NumberOfWorkerProcess or Thread shoud be >= 1>\n");
    exit(-1);
  }

  numberOfThreads=atoi(argv[2]);
  if(numberOfThreads<1){
   printf("Number of threads should be > 1 \n");
   exit(-1);
  }

  Worker *worker=NULL;
  worker = malloc(sizeof(Worker)*numberOfThreads);
  if(worker == NULL){
   printf("\r\nWorker Memory Allocation Failed\r\n");
   exit(-1);
  }
  memset(worker,0,sizeof(Worker)*numberOfThreads);

  fp = fopen(argv[1],"r");
  if(fp == NULL){
     printf("\n File Open Failed \n");
     exit(0);
  }

  numberOfLines = getNumberOfLines(fp);
  
  /*Memory Allocation for the Word List,task and Word Length Array*/
  fileContent = malloc(sizeof(char*)*numberOfLines);
  wordLength  = (long*) malloc(sizeof(long)*numberOfLines);
  task = malloc(sizeof(Tasks)*NUMBER_OF_TASKS);

  if(fileContent == NULL){
   printf("\r\n Malloc for fileContent Failed 1 \r\n");
   exit(-1);
  }
  if(wordLength == NULL){
   printf("\r\n Malloc for wordLength Failed 1 \r\n");
   exit(-1);
  }
  if(task == NULL){
   printf("\r\n Malloc for task Failed 1 \r\n");
   exit(-1);
  }

  memset(fileContent,0,sizeof(char*)*numberOfLines);
  memset(wordLength,0,sizeof(long)*numberOfLines);
  memset(task,0,sizeof(Tasks)*NUMBER_OF_TASKS);

  for(index=0; index < numberOfLines ;index++){
     fileContent[index] = malloc(sizeof(char)*LINE_LENGTH);
     if(fileContent[index] == NULL){
         printf("\r\n Malloc for fileContent Failed at index %d \r\n",index);
        exit(-1);
     }
     memset(fileContent[index],0,sizeof(char)*LINE_LENGTH);
  }

  
  taskWordIndex = malloc(sizeof(long*)*NUMBER_OF_TASKS);  
  taskUniqueWordIndex = malloc(sizeof(long*)*NUMBER_OF_TASKS);  
  if(taskWordIndex == NULL){
   printf("\r\n Malloc for taskWordIndex Failed 1 \r\n");
   exit(-1);
  }
  if(taskUniqueWordIndex == NULL){
   printf("\r\n Malloc for taskUniqueWordIndex Failed 1 \r\n");
   exit(-1);
  }
  memset(taskWordIndex,0,sizeof(long*)*NUMBER_OF_TASKS);
  memset(taskUniqueWordIndex,0,sizeof(long*)*NUMBER_OF_TASKS);

   for(index=0;index<NUMBER_OF_TASKS;index++){
    taskWordIndex[index]=malloc(sizeof(long)*numberOfLines); 
    taskUniqueWordIndex[index]=malloc(sizeof(long)*numberOfLines); 
    if(taskWordIndex[index]==NULL){
       printf("\r\n Malloc for taskWordIndex Failed at index %d \r\n",index);
       exit(-1);
    }
    if(taskUniqueWordIndex[index]==NULL){
       printf("\r\n Malloc for taskUniqueWordIndex Failed at index %d \r\n",index);
       exit(-1);
    }
    memset(taskUniqueWordIndex[index],0,sizeof(long)*numberOfLines);
    memset(taskWordIndex[index],0,sizeof(long)*numberOfLines);
   }


  /*Two Operation performed in getFileContentUpdateTaskWordCount Function to reduce 
  * the reading file content twice*/
  totalWordCount=getTotalWordCountAndUpdateTaskWordCount(fp,fileContent,task,numberOfLines,taskWordIndex);
   
   /*Assign the Tasks to the Worker*/

   minNumberOfTaskPerProcess = NUMBER_OF_TASKS/numberOfThreads;
   remainingProcess = NUMBER_OF_TASKS % numberOfThreads;

   for(workerIndex=0;workerIndex<numberOfThreads;workerIndex++){
       worker[workerIndex].numberOfTasks = minNumberOfTaskPerProcess;
   }

   for(workerIndex=0;workerIndex<remainingProcess;workerIndex++){
       worker[workerIndex].numberOfTasks++;
   }

   int lastWorkerIndex = numberOfThreads-1;
   worker[lastWorkerIndex].startTaskIndex = NUMBER_OF_TASKS - worker[lastWorkerIndex].numberOfTasks;
   worker[0].startTaskIndex = 0;
   /*UPdate Start Index of Remaining tasks*/
   for(workerIndex = lastWorkerIndex-1; workerIndex>0; workerIndex-- ){
      worker[workerIndex].startTaskIndex = worker[workerIndex+1].startTaskIndex - worker[workerIndex].numberOfTasks;
   }
 
   for(workerIndex=0; workerIndex < numberOfThreads; workerIndex++){
       worker[workerIndex].task = task;
   }
  
  for(taskIndex=0; taskIndex < NUMBER_OF_TASKS; taskIndex++){
          task[taskIndex].wordList = fileContent;
          task[taskIndex].wordLength = wordLength;
          task[taskIndex].taskWordIndex = taskWordIndex[taskIndex];
          task[taskIndex].uniqueWordIndex = taskUniqueWordIndex[taskIndex];
  }


  int create_error=0;
  timing_start();
   pthread_t threads[numberOfThreads]; 
   for(workerIndex=0;workerIndex<numberOfThreads;workerIndex++){
   create_error = pthread_create(&threads[workerIndex], NULL, workerProcess_run, (void *)&worker[workerIndex]);
   if(create_error){
        printf("pthread_create 0 returned error: %d\n", create_error);
        exit(-1);
    }
   }

  int join_error=0;

  for (taskIndex = 0; taskIndex<numberOfThreads; ++taskIndex){
      join_error = pthread_join(threads[taskIndex], NULL);
      if(join_error){
         printf("pthread_join %d returned error: %d\n",taskIndex ,join_error);
         pthread_exit(NULL);
      }
  }
   
  for(workerIndex=0;workerIndex<numberOfThreads;workerIndex++){
    totalUniqueWordCount += worker[workerIndex].uniqueWordCount;
  }
  printf("Total Number of UniqueWordFound = %ld \n",totalUniqueWordCount);
  
  for(workerIndex=0;workerIndex<numberOfThreads;workerIndex++){
    printf("Worker %d has Found %d UniqueWords\n",workerIndex,worker[workerIndex].uniqueWordCount);
  }
  
   timing_stop();
   printf("\r\n Ellapsed Time for Compute Phase:->");
   print_timing();


    
  outPutFilePtr = fopen("outPutFileParallel.txt","w");
  if(outPutFilePtr == NULL){
     printf("\n File Open Failed at outPutFilePtr \n");
     exit(0);
  }

  writeToOutPutFile(outPutFilePtr,taskUniqueWordIndex,fileContent,task);

  if(outPutFilePtr!=NULL){
    fclose(outPutFilePtr);
  }
  
  for(index=0;index<numberOfLines;index++){
   if(fileContent[index]!=NULL)
      free(fileContent[index]);
  }
  if(worker!=NULL){
     free(worker);
  }
  for(taskIndex=0;taskIndex<NUMBER_OF_TASKS;taskIndex++){
     if(taskWordIndex[taskIndex]!=NULL)
     free(taskWordIndex[taskIndex]);
     if(taskUniqueWordIndex[taskIndex]!=NULL)
     free(taskUniqueWordIndex[taskIndex]);
  }
   

  if(taskWordIndex!=NULL)
  free(taskWordIndex);

  if(taskUniqueWordIndex!=NULL)
  free(taskUniqueWordIndex);

  if(fileContent!=NULL)
  free(fileContent);

  if(wordLength!=NULL)
  free(wordLength);

  if(task!=NULL)
  free(task);

  if(fp!=NULL)
  fclose(fp);

  pthread_exit(NULL);
  return 0;

}
