all:
	gcc -g stackSerial.c timing.c GetNumbers.c  -o stackSerial.out
	gcc -g stackParallelThread.c timing.c GetNumbers.c -o stackParallelThread.out -lpthread

part_3_Serial:
	
	gcc -g stackSerial.c timing.c GetNumbers.c -o stackSerial.out
part_3_Parallel:
	
	gcc -g stackParallelThread.c timing.c GetNumbers.c -o stackParallelThread.out -lpthread

clean:
	rm ./*.out
