CXX=clang++
FLAGS=-Wall

all: main

main: main.cpp tsp.cpp
	$(CXX) -o $@ $^ $(FLAGS)

.PHONY: clean
clean:
	rm main
