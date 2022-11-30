#include "graph.h"

using namespace std;

Graph::Graph(int V, int initial_vertex,
			 bool random_graph, char *output) // constructor of Graph
{
  if (V < 1) {
	// checks if number of vertexes is less than 1
	cout << "Error: number of vertexes <= 0\n";
	exit(1);
  }

  this->V = V;							 // assigns the number of vertices
  this->initial_vertex = initial_vertex; // assigns initial vertex
  this->total_edges = 0;				 // initially the total of edges is 0
  this->fp = NULL;

  if(output != NULL)
	  this->fp = fopen(output, "w");

  if (random_graph)
	generatesGraph();
}

Graph::~Graph() {
	fclose(fp);
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

  if(fp) {
	  showInfoGraph();
	  showGraph();
  }
}

/*
	print number of nodes and edges into file
*/
void Graph::showInfoGraph() {
  fprintf(fp, "%d %ld\n", V, map_edges.size());
}

/*
	print all edges into a file
*/
void Graph::showGraph()
{
  map<pair<int, int>, int>::iterator it;
  for (it = map_edges.begin(); it != map_edges.end(); ++it)
	  fprintf(fp, "%d %d %d\n", it->first.first, it->first.second, it->second);
}

void Graph::addEdge(int src, int dest, int weight) // add a edge
{
	map_edges[make_pair(src, dest)] = weight; // adds edge in the map
}

int Graph::existsEdge(int src, int dest) // checks if exists a edge
{
  map<pair<int, int>, int>::iterator it = map_edges.find(make_pair(src, dest));

  if (it != map_edges.end())
	return it->second; // returns cost
  return -1;
}


PYBIND11_MODULE(_graph, g)
{
    g.doc() = "TSP graph generation";

	pybind11::class_<Graph>(g, "Graph")
		.def(pybind11::init<int, int, bool, char*>());

    g.def("generatesGraph", &Graph::generatesGraph);
}
