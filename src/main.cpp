#include <iostream>
#include <unistd.h>

#include "tsp.h"
#include "graph.h"

using namespace std;

int main(int argc, char **argv) {
  srand(time(NULL));

  int num = 50, thread_num = 1;
  char opt;
  // get opt
  while ((opt = getopt(argc, argv, "t:n:")) != -1) {
    switch (opt) {
    case 'n':
      num = atoi(optarg);
      break;
    case 't':
      thread_num = atoi(optarg);
      break;
    case '?':
      return 1;
    }
  }

  // generate graph randomly
  Graph *g = new Graph(num, 0, true);
  // g->showInfoGraph();

  // parameters: the graph, population size, generations and mutation rate
  // optional parameters: show_population
  Genetic genetic(g, 10, 1000, 5, false);

  clock_t begin_time = clock(); // gets time
  genetic.run();                // runs the genetic algorithm
  cout << "\n\nTime for to run the genetic algorithm: "
       << float(clock() - begin_time) / CLOCKS_PER_SEC
       << " seconds."; // shows time in seconds

  Genetic genetic2(g, 10, 10000, 5, false);

  begin_time = clock(); // gets time
  genetic2.run();       // runs the genetic algorithm
  cout << "\n\nTime for to run the genetic algorithm: "
       << float(clock() - begin_time) / CLOCKS_PER_SEC
       << " seconds."; // shows time in seconds

  return 0;
}
