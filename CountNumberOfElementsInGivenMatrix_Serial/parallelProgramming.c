#include<stdio.h>
#include<stdlib.h>
#include "timing.h"

void SumUpto(void*  parNumber){
   int i=0;
   int sum=0;
   int number=(int) parNumber;
   for(i=0;i<=number;i++){
    sum+=i;
   }
   printf("\r\n Parallel Programming Sum = %d\r\n", sum);
}

int main(int argc, char*argv[]){
      timing_start();
      int n=0;
      int p =2;
      int i=0;
      int create_error =0;
      int join_error=0;

      if(argc < 2){
          perror("\nUsage ./parallelProgramming.out <N> <option>\n");
          exit(-1);
      }
      else if(argc >= 3){
		p = atoi(argv[2]);
       }
      n=atoi(argv[1]);
      pthread_t threads[p];
    
      for (i = 0; i < p; ++i){
	create_error = pthread_create(&threads[i], NULL, SumUpto, (void *)n);
	if(create_error){
		printf("pthread_create returned error: %d\n", create_error);
		exit(-1);
	}
      }

      for (i = 0; i < p; ++i){
	join_error = pthread_join(threads[i], NULL);
	if(join_error){
		printf("pthread_join returned error: %d\n", join_error);
		exit(-1);
	}
      }

      timing_stop();
      print_timing();
      pthread_exit(NULL);
      return 0;
}
