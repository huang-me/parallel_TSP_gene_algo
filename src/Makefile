CXX=g++
CC=gcc
FLAGS=-Wall -g -pthread
OBJS=graph.o tsp.o tsp_pthread.o main.o
HEADERS=graph.h tsp.h tsp_pthread.h

all: main

%.o: %.cpp $(HEADERS)
	$(CXX) -o $@ $< -c $(FLAGS)

main: $(OBJS) $(HEADERS)
	$(CXX) -o $@ $(OBJS) $(FLAGS)

.PHONY: clean
clean:
	rm -f main *.o
