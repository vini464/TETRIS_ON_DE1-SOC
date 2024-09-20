compile: accel.o
	gcc ./src/main.c -I./headers ./obj/accel.o -o run

accel.o:
	gcc -c .src/accel.c
	mv accel.o ./obj