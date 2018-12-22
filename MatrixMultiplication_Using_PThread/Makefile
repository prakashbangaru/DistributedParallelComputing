all:
	gcc matrixMultiplicationSerial.c timing.c -o matrixMultiplicationSerial.out -lm
	gcc matrixMultiplicationParallel.c timing.c -o matrixMultiplicationParallel.out -lm -lpthread

part_1_serial:
	
	gcc matrixMultiplicationSerial.c timing.c -o matrixMultiplicationSerial.out -lm

part_2_parallel:
	
	gcc matrixMultiplicationParallel.c timing.c -o matrixMultiplicationParallel.out -lm -lpthread
clean:
	rm ./*.out
