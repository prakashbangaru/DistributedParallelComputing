all:
	gcc -g BagOfTaskSerial.c timing.c -o BagOfTaskSerial.out -lm
	gcc -g BagOfTaskParallel.c timing.c -o BagOfTaskParallel.out -lm -lpthread

part_1_Serial:
	
	gcc -g BagOfTaskSerial.c timing.c -o BagOfTaskSerial.out -lm

part_1_Parallel:
	
	gcc -g BagOfTaskParallel.c timing.c -o BagOfTaskParallel.out -lm -lpthread

clean:
	rm ./*.out
