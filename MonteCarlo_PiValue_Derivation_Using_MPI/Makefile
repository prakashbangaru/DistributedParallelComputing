makeall:
	mpicc -o part_2_parallel.out MonteCarloParallelMPI.c timing.c -fopenmp
	gcc -o part_1_serial.out MonteCarloSerialMPI.c timing.c

part_1_serial:
	gcc -o part_1_serial.out MonteCarloSerialMPI.c timing.c

part_2_parallel:
	mpicc -o part_2_parallel.out MonteCarloParallelMPI.c timing.c -fopenmp

clean:
	rm -f *.o  *.out

