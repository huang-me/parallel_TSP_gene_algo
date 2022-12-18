CXX := g++
FLAGS := -lpthread -Wall
EXECS = main

.PHONY: all clean
all: $(EXECS)

$(EXECS): %: %.cpp test.h
	$(CXX) $< -o $@ $(FLAGS) 

clean:
	rm -f $(EXECS)
