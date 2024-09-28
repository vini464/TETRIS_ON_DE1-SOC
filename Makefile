source_libs: 
	gcc -c ./src/libs/*.c -lintelfpgaup
	mv *.o ./obj

game: source_libs
	gcc ./src/main.c -I./headers ./obj/*.o -o tetriz -lpthread -lintelfpgaup
	sudo ./tetriz

clear_objects:
	rm ./obj/*.o

run: ./tetriz
	sudo ./tetriz
