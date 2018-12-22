all:
	gcc -g MatrixMultiplicationParallelOMP.c timing.c -o MatrixMultiplicationParallelOMP.out -fopenmp
	gcc -g MatrixMultiplicationSerial.c timing.c -o MatrixMultiplicationSerial.out -lm
part_1_Serial:
	
	gcc -g MatrixMultiplicationSerial.c timing.c -o MatrixMultiplicationSerial.out -lm
part_2_Parallel_OMP:
	
	gcc -g MatrixMultiplicationParallelOMP.c  timing.c -o MatrixMultiplicationParallelOMP.out -fopenmp

clean:
	rm ./*.out
