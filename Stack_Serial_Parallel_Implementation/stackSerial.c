#include<stdio.h>
#include<stdlib.h>
#include<string.h>

extern int *numbers;
extern int numSize;

typedef struct {
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

int GetStackCount(Stack stack, Pop pop);
void pushElement(Stack *parStack);
void popElement(Pop *parPop);



int main(int argc, char*argv[]){
        timing_start();	
	int *stackArray = NULL;
	int *poppedElements=NULL;
	int numberOfLoops = 0;
	int totalNumberOfStackElements = 0;
	int index = 0;
	
	if(argc < 3){
		printf(" ./stackSerial.c <InputFile> <NumberOfIterations>>\r\n ");
		exit(-1);
        }
	
	GetNumbers(argv[1]);
	numberOfLoops = atoi(argv[2]);

	
	totalNumberOfStackElements = numSize*numberOfLoops;

	if(numberOfLoops < 1){
		printf("Number of Loops should be atleast 1\r\n");
		exit(-1);
	}
	
	
	stackArray = (int*)malloc(sizeof(int)*totalNumberOfStackElements);
	
	if(stackArray == NULL){
		printf("Memory Allocation Failed for stackArray \r\n");
		exit(-1);
	}
	
	poppedElements = (int*)malloc(sizeof(int)*totalNumberOfStackElements);
	if(poppedElements == NULL){
		printf("Memory Allocation Failed for poppedElements \r\n");
		exit(-1);
	}
	
	
	memset(stackArray,-1,sizeof(int)*totalNumberOfStackElements);
	memset(poppedElements,-1,sizeof(int)*totalNumberOfStackElements);
	
	
	
        Stack stack;
	Pop pop; /*Stores the Elements that are Popped*/
	
	memset(&stack,0,sizeof(Stack));
	memset(&pop,0,sizeof(Pop));
	
        stack.stackSize = totalNumberOfStackElements;
        stack.stackArray = stackArray;
	
	pop.poppedElements = poppedElements;
	pop.totalElementsToBePopped = totalNumberOfStackElements; 
	pop.stackArray =  stackArray;
        
        pushElement(&stack);
        popElement(&pop);

	printf("Stack Count = %d\r\n",GetStackCount(stack,pop));
        
        #if 0
        for(index = 0 ; index < totalNumberOfStackElements ;index++){
             printf("%d\r\n",poppedElements[index]);
        }
        #endif

   if(stackArray != NULL){
      free(stackArray);
   }

   if(poppedElements!=NULL){
     free(poppedElements);
   }

   timing_stop();
   print_timing();
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

int GetStackCount(Stack stack, Pop pop){
	
        int currentStackCount= 0;
	currentStackCount = stack.currentNumberOfElements - pop.numberOfElementsPopped;	
	return currentStackCount;
}

void pushElement(Stack *parStack){
	Stack *stack = parStack;
	int currentStackIndex = 0;
	int index = 0;
	int stackIndex = 0;
	
	while(stackIndex < stack->stackSize){
	     for(index=0; index < numSize; index++){
		     stack->stackArray[stackIndex] = numbers[index];
	             stack->currentNumberOfElements++;
		     stackIndex++;
                     if(stackIndex >= stack->stackSize){
                        break;
                     } 
	     }
	}
}

void popElement(Pop *parPop){
	Pop *pop = parPop;
	int popIndex = 0;
	int elementIndex = 0;
	popIndex = pop->totalElementsToBePopped-1; 
	
	while(popIndex >= 0){
     	   if(pop->stackArray[popIndex] != -1){
              pop->poppedElements[elementIndex] = pop->stackArray[popIndex];
              pop->stackArray[popIndex] = -1;
              pop->numberOfElementsPopped++;
              popIndex--;
              elementIndex++;
          }
    }
}
	
