

source_libs: 
	gcc -c ./src/libs/*.c
	mv *.o ./obj

game_test: source_libs
	gcc ./src/main.c -I./headers ./obj/*.o -o run
	./run
