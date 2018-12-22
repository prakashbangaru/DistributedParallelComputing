#include<stdio.h>
#include<stdlib.h>
#include "timing.h"

void SumUpto(int number){
   int i=0;
   int sum=0;
   for(i=0;i<=number;i++){
    sum+=i;
   }
   printf("\r\n Serial Programming Sum = %d\r\n", sum);
}

int main(int argc, char*argv[]){
      timing_start();
      int n=0;
      int p =2;
      int i=0;

      if(argc < 2){
          perror("\nUsage ./serialProgramming.out <N> <option>\n");
          exit(-1);
      }
      else if(argc >= 3){
		p = atoi(argv[2]);
       }
      n=atoi(argv[1]);

      for(i=0;i<p;i++){
       SumUpto(n);
      }      
      timing_stop();
      print_timing();
      return 0;
}
