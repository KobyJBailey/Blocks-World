all: run

State.o: State.h State.cpp
	g++ -g -w -std=c++11 -c State.cpp

Node.o: Node.h Node.cpp
	g++ -g -w -std=c++11 -c Node.cpp 

run: main.cpp Node.o State.o 
	g++ -g -w -std=c++11 -o blocksworld main.cpp Node.o State.o -lpthread -lrt

clean: 
	rm -rf *.o blocksworld