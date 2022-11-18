#include "graph.h"

using namespace std;

Graph::Graph(int V, int initial_vertex,
             bool random_graph) // constructor of Graph
{
  if (V < 1) {
    // checks if number of vertexes is less than 1
    cout << "Error: number of vertexes <= 0\n";
    exit(1);
  }

  this->V = V;                           // assigns the number of vertices
  this->initial_vertex = initial_vertex; // assigns initial vertex
  this->total_edges = 0;                 // initially the total of edges is 0

  if (random_graph)
    generatesGraph();
}

void Graph::generatesGraph() {
  vector<int> vec;

  // creates the vector
  for (int i = 0; i < V; i++)
    vec.push_back(i);

  // generates a random permutation
  random_shuffle(vec.begin(), vec.end());

  initial_vertex = vec[0]; // updates initial vertex

  int i, weight;
  for (i = 0; i <= V; i++) {
    weight = rand() % V + 1; // random weight in range [1,V]

    if (i + 1 < V)
      addEdge(vec[i], vec[i + 1], weight);
    else {
      // add last edge
      addEdge(vec[i], vec[0], weight);
      break;
    }
  }

  // add others edges randomly

  // int limit_edges = V * (V - 1) / 2; // calculates the limit of edges
  // int size_edges = limit_edges - rand() % (V / 10);
  //
  // for (int i = 0; i < size_edges; i++) {
  //   int src = rand() % V;    // random source
  //   int dest = rand() % V;   // random destination
  //   weight = rand() % V + 1; // random weight in range [1,V]
  //   if (src != dest) {
  //     addEdge(vec[src], vec[dest], weight);
  //     addEdge(vec[dest], vec[src], weight);
  //   }
  // }
  int maxWeight = V * 2;
  for (int i = 0; i < V; ++i) {
    for (int j = i + 1; j < V; ++j) {
      if (rand() % 30 == 0)
        continue;
      int weight = rand() % maxWeight + 1;
      addEdge(vec[i], vec[j], weight);
      addEdge(vec[j], vec[i], weight);
    }
  }
}

void Graph::showInfoGraph() {
  cout << "Showing info of graph:\n\n";
  cout << "Number of vertices: " << V;
  cout << "\nNumber of edges: " << map_edges.size() << "\n";
}

void Graph::addEdge(int src, int dest, int weight) // add a edge
{
  map_edges[make_pair(src, dest)] = weight; // adds edge in the map
}

void Graph::showGraph() // shows all connections of the graph
{
  map<pair<int, int>, int>::iterator it;
  for (it = map_edges.begin(); it != map_edges.end(); ++it)
    cout << it->first.first << " linked to vertex " << it->first.second
         << " with weight " << it->second << endl;
}

int Graph::existsEdge(int src, int dest) // checks if exists a edge
{
  map<pair<int, int>, int>::iterator it = map_edges.find(make_pair(src, dest));

  if (it != map_edges.end())
    return it->second; // returns cost
  return -1;
}
