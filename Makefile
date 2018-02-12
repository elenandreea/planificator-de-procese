.PHONEY: build run clean

build: planificator

planificator : planific1.c 
	gcc -Wall -g planific1.c -o planificator

clean: 
	rm -rf planificator
