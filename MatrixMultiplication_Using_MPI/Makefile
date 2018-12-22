makeall:
	mpicc -o part_2_parallel.out parallelMultiplicationMpi.c timing.c -fopenmp
	gcc -o generateInputMatrixFile.out generateInputMatrixFile.c
	gcc -g MatrixMultiplicationSerial.c timing.c -o part_1_serial.out -lm

part_1_serial:
	gcc -g MatrixMultiplicationSerial.c timing.c -o part_1_serial.out -lm

part_2_parallel:
	mpicc -o part_2_parallel.out parallelMultiplicationMpi.c timing.c -fopenmp
	gcc -o generateInputMatrixFile.out generateInputMatrixFile.c

clean:
	rm -f *.o output*.txt *.out

