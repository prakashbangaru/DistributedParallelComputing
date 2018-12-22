all:
	gcc -g distributionValueParallel.c timing.c -o distributionValueParallel.out -lpthread
	gcc -g distributionValueSerial.c timing.c -o distributionValueSerial.out

part_1_parallel:
	gcc -g distributionValueParallel.c timing.c -o distributionValueParallel.out -lpthread
	
part_1_serial:
	gcc -g distributionValueSerial.c timing.c -o distributionValueSerial.out

clean:
	rm ./*.out
