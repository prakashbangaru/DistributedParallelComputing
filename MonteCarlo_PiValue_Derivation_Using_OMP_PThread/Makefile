all:
	gcc -g MonteCarloSerial.c timing.c -o MonteCarloSerial.out
	gcc -g MonteCarloParallelOMP.c timing.c -o MonteCarloParallelOMP.out -fopenmp
	gcc -g MonteCarloParallelThread.c timing.c -o MonteCarloParallelThread.out -lpthread
part_1_Serial:
	
	gcc -g MonteCarloSerial.c timing.c -o MonteCarloSerial.out
part_2_Parallel_OMP:
	
	gcc -g MonteCarloParallelOMP.c timing.c -o MonteCarloParallelOMP.out -fopenmp
part_3_Parallel_PThread:
	
	gcc -g MonteCarloParallelThread.c timing.c -o MonteCarloParallelThread.out -lpthread

clean:
	rm ./*.out
