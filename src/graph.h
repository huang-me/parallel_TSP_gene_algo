#ifndef GRAPH_H
#define GRAPH_H

#include <algorithm> // random_shuffle
#include <iostream>
#include <map>
#include <utility>
#include <vector>

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/operators.h>

// class that represents the graph
class Graph {
private:
  int V;                                        // number of vertices
  int total_edges;                              // total of edges
  int initial_vertex;                           // initial vertex
  FILE *fp;
  std::map<std::pair<int, int>, int> map_edges; // map of the edges
public:
  Graph(int V, int initial_vertex, bool random_graph = false, char *output = NULL); // constructor
  ~Graph();
  void addEdge(int v1, int v2, int weight);                    // adds a edge
  void showGraph();                  // shows all the links of the graph
  void generatesGraph();             // generates a random graph
  void showInfoGraph();              // shows info of the graph
  int existsEdge(int src, int dest); // checks if exists a edge
  friend class Genetic;              // to access private membres this class
  friend class Genetic_thread;       // to access private membres this class
};

#endif
