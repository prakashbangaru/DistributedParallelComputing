all:
	gcc serialProgramming.c timing.c -o serialProgramming.out -lm
	gcc parallelProgramming.c timing.c -o parallelProgramming.out -lpthread

part_1_serial:
	
	gcc serialProgramming.c timing.c -o serialProgramming.out -lm

part_1_parallel:
	
	gcc parallelProgramming.c timing.c -o parallelProgramming.out -lpthread
clean:
	rm ./*.out
