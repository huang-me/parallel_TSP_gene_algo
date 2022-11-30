CXX=g++
CC=gcc
PYINCLUDE = /usr/include/python3.8
PYFLAGS = -lpython3.8
PYBIND=`python3 -m pybind11 --includes`
PYCONFIG=`python3-config --extension-suffix`
FLAGS=-Wall -g -pthread -fPIC -shared
LIBS=_graph _Genetic

all: $(LIBS)

_graph: graph.cpp graph.h
	$(CXX) -I$(PYINCLUDE) $(PYBIND) $(FLAGS) $< -o $@$(PYCONFIG) $(PYFLAGS)

_Genetic: tsp_pthread.cpp graph.cpp
	$(CXX) -I$(PYINCLUDE) $(PYBIND) $(FLAGS) $^ -o $@$(PYCONFIG) $(PYFLAGS)

.PHONY: clean
clean:
	rm -f *.so
