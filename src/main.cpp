#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "graph.h"
#include "tsp.h"
#include "tsp_pthread.h"

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

  struct timespec start, finish;

  // parameters: the graph, population size, generations and mutation rate
  // optional parameters: show_population
  Genetic genetic(g, 10, 10000, 5, false);

  clock_gettime(CLOCK_MONOTONIC, &start);
  genetic.run(); // runs the genetic algorithm
  clock_gettime(CLOCK_MONOTONIC, &finish);
  cout << "\n\nTime for running the single threaded genetic algorithm: "
       << (finish.tv_sec - start.tv_sec) +
              float(finish.tv_nsec - start.tv_nsec) / 1e9
       << " seconds.\n"; // shows time in seconds

  Genetic_thread genetic2(g, 10, 10000, 5, thread_num, false);

  clock_gettime(CLOCK_MONOTONIC, &start);
  genetic2.run(); // runs the genetic algorithm
  clock_gettime(CLOCK_MONOTONIC, &finish);
  cout << "\n\nTime for running the genetic algorithm with "
			 << thread_num << " threads: "
       << (finish.tv_sec - start.tv_sec) +
              float(finish.tv_nsec - start.tv_nsec) / 1e9
       << " seconds."; // shows time in seconds

  return 0;
}
