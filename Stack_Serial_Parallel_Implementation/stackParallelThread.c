#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern int *numbers;
extern int numSize;

typedef struct {
	int startIndex;
	int endIndex;
	int stackSize;
	int *stackArray;
	int currentNumberOfElements;
}Stack;

typedef struct{
	int *poppedElements;
	int numberOfElementsPopped;
	int totalElementsToBePopped;
	int *stackArray;
}Pop;

int GetStackCount(Stack *stack, Pop pop, int numberOfPushThreads);
void pthreadPush(void *parStack);
void pthreadPop(void *parPop);


void sample(){
}

int main(int argc, char*argv[]){
        timing_start();
	int *stackArray = NULL;
	int *poppedElements=NULL;
	int numberOfLoops = 0;
	int numberOfThreads = 0;
	int numberOfPopThread = 0;
	int numberOfPushThread = 0;
	int totalNumberofStackElements = 0;
	int index = 0;
	int threadStackSize = 0;
	int remainingElements = 0;
	int createError = 0;
	int joinError = 0;
	
	if(argc < 4){
		printf(" ./Stack.out <InputFile> <NumberOfIterations> <NumberOfThreads(Should be greaterthan 2)>\r\n ");
		exit(-1);
        }

	GetNumbers(argv[1]);
	numberOfLoops = atoi(argv[2]);
	numberOfThreads = atoi(argv[3]);

	if(numberOfLoops < 1){
		printf("Number of Loops should be atleast 1\r\n");
		exit(-1);
	}
	
	if(numberOfThreads <= 2){
	    printf("Number Of Threads should be atleast greater than 2\r\n");    
        exit(-1);		
	}
	
	stackArray = (int*)malloc(sizeof(int)*numberOfLoops*numSize);
	
	if(stackArray == NULL){
		printf("Memory Allocation Failed for stackArray \r\n");
		exit(-1);
	}
	
	poppedElements = (int*)malloc(sizeof(int)*numberOfLoops*numSize);
	if(poppedElements == NULL){
		printf("Memory Allocation Failed for poppedElements \r\n");
		exit(-1);
	}
	
	
	memset(stackArray,-1,sizeof(int)*numberOfLoops*numSize);
	memset(poppedElements,-1,sizeof(int)*numberOfLoops*numSize);
	
	
	numberOfPopThread = 1;
	numberOfPushThread = numberOfThreads-1;
	pthread_t push_threads[numberOfPushThread];
	pthread_t pop_threads;
        Stack stack[numberOfPushThread];
	Pop pop; /*Stores the Elements that are Popped*/
	
	memset(stack,0,sizeof(Stack)*numberOfPushThread);
	memset(&pop,0,sizeof(Pop));
	
	totalNumberofStackElements = numSize*numberOfLoops;
	threadStackSize = (totalNumberofStackElements/numberOfPushThread); 
	remainingElements = totalNumberofStackElements % numberOfPushThread;
	
	pop.poppedElements = poppedElements;
	pop.totalElementsToBePopped = totalNumberofStackElements; 
	pop.stackArray =  stackArray;
	
	/*Assign each thread the number of Elements it has to push into the stack and Assign stack Pointer to each thread*/
	for(index = 0;index < numberOfPushThread; index++){	
	  stack[index].stackSize=threadStackSize;
	  if(remainingElements != 0){
			stack[index].stackSize++;
			remainingElements--;
		}
		stack[index].stackArray = stackArray;
	}
	
	stack[0].startIndex = 0;
	stack[0].endIndex = stack[0].stackSize-1;
	

	/*Assign Start and End Index of the stack that each thread handles*/
	for(index = 1;index < numberOfPushThread; index++){
              stack[index].startIndex = stack[index-1].endIndex + 1;	
              stack[index].endIndex = stack[index-1].endIndex + stack[index].stackSize;	   
	}
       	
 	/*Spawning the threads to push the elements*/
	for(index = 0;index < numberOfPushThread; index++){
		createError = pthread_create(&push_threads[index], NULL, pthreadPush, (void *)&stack[index]);
		if (createError){
			printf("error creating pthread for Pushing error code: %d\n", createError);
			exit(-1);
		}
	}
	
	/*Spawning the threads to pop the elements*/
	createError = pthread_create(&pop_threads, NULL, pthreadPop, (void *)&pop);
        if (createError){
	    printf("error creating pthread for Popping error code: %d\n", createError);
	    exit(-1);
	}
	
	/*Joining the threads that pushes the Elements into the stack*/
	for(index = 0;index < numberOfPushThread; index++){
	joinError = pthread_join(push_threads[index], NULL);
		if (joinError){
			printf("error in pthread_join error code: %d\n", joinError);
			exit(-1);
		}
	}
	
	/*Joining the thread that pop the Element from the stack*/
	joinError = pthread_join(pop_threads, NULL);
	if (joinError){
	  printf("error in pthread_join error code: %d\n", joinError);
	  exit(-1);
	}

	printf("Stack Count After Popping All Elements= %d\r\n",GetStackCount(stack,pop,numberOfPushThread));
        
        #if 0
        for(index = 0 ; index < totalNumberofStackElements ;index++){
        printf("%d\r\n",poppedElements[index]);        
        }
        #endif


   if(poppedElements!=NULL){
     free(poppedElements);
   }

   if(stackArray != NULL){
      free(stackArray);
   }

   timing_stop();
   print_timing();

   pthread_exit(NULL);
   return 0;   
}

/*************************************************************************************\
*    Function : GetStackCount                          *******************************\
*    Param: 1. A part of a stack handled by the thread *******************************\
*           2. Pop Structure contains Elements that popped and their Count ***********\ 
*           3. number of Threads that performs push Operation ************************\
*    Description : This function gives correct current stack size when it is 
*                  called after the thread join call. It may give minor error value
*				   when it is called in the middle of the thread Operations.
**************************************************************************************/

int GetStackCount(Stack *stack, Pop pop, int numberOfPushThread){
	int index=0;
	int pushStackCount=0;
	int currentStackCount= 0;
	
	for(index = 0;index < numberOfPushThread; index++){
	   pushStackCount = pushStackCount + stack[index].currentNumberOfElements;	
	}
	
	currentStackCount = pushStackCount - pop.numberOfElementsPopped;	
	return currentStackCount;
}

void pthreadPush(void *parStack){
	Stack *stack = (Stack*)parStack;
	int currentStackIndex = 0;
	int index = 0;
	int initIndexValue = 0;
	int stackIndex = stack->startIndex;

        stack->currentNumberOfElements = 0;	
	index = (stack->startIndex % numSize);
	
	while(stackIndex <= stack->endIndex){
	     for(; index < numSize; index++){
		     stack->stackArray[stackIndex] = numbers[index];
	             stack->currentNumberOfElements++;
		     stackIndex++;
                     if(stackIndex > stack->endIndex){
                       break;
                     }
	     }
		 index = 0;
	}
}

void pthreadPop(void *parPop){
	Pop *pop = (Pop*)parPop;
	int popIndex = 0;
	int elementIndex = 0;

        pop->numberOfElementsPopped=0;
	popIndex = pop->totalElementsToBePopped-1; 
	
	while(popIndex >= 0){
		while(pop->stackArray[popIndex] == -1){
			//Wait for the stack to be get filled in the pointed Index;
		}
     	if(pop->stackArray[popIndex] != -1){
		  pop->poppedElements[elementIndex] = pop->stackArray[popIndex];
                  pop->stackArray[popIndex] = -1;
		  pop->numberOfElementsPopped++;
                  popIndex--;
		  elementIndex++;
	    }
    }
}
	
