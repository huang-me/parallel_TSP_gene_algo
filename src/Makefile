CXX=clang++
FLAGS=-Wall
OBJS=graph.o tsp.o main.o

all: main

%.o: %.cpp
	$(CXX) -o $@ $< -c $(FLAGS)

main: $(OBJS)
	$(CXX) -o $@ $^ $(FLAGS)

.PHONY: clean
clean:
	rm main *.o
