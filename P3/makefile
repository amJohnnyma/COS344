#add all the files that you need to the files makefile variable below as a space seperated list
files = 

main: src/main.cpp src/glad.c
	g++ -g src/shader.cpp $(files) src/main.cpp src/glad.c -lglfw -pthread -lGLEW -ldl -lGL -o main

clean:
	rm -f *.o main

run:
	./main

all:
	make clean
	make
	make run