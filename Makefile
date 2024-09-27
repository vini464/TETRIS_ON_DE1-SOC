

source_libs: 
	gcc -c ./src/libs/*.c 
	mv *.o ./obj

game_test: source_libs
	gcc ./src/main.c -I./headers ./obj/*.o -o run -lpthread -lintelfpgaup
	sudo ./run

clear_objects:
	rm ./obj/*.o
