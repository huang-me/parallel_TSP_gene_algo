#ifndef TSP_H
#define TSP_H

#include <set>

#include "graph.h"
#include "sort.h"

// class that represents genetic algorithm
class Genetic {
private:
  Graph *graph; // the graph
  std::vector<my_pair>
      population;           // each element is a pair: vector and total cost
  int size_population;      // size of population
  int real_size_population; // real size population
  int generations;          // amount of generations
  int mutation_rate;        // mutation rate
  bool show_population;     // flag to show population
private:
  void initialPopulation(); // generates the initial population
  bool _findInitial(std::vector<int>&, int, std::vector<bool>&, int&);
public:
  Genetic(Graph *graph, int amount_population, int generations,
          int mutation_rate, bool show_population = true); // constructor
  int isValidSolution(
      std::vector<int> &solution); // checks if a solution is valid
  void showPopulation();           // shows population
  void crossOver(std::vector<int> &parent1,
                 std::vector<int> &parent2); // makes the crossover
  void insertBinarySearch(std::vector<int> &child,
                          int total_cost); // uses binary search to insert
  void run();                              // runs genetic algorithm
  int getCostBestSolution();               // returns cost of the best solution
  bool existsChromosome(
      const std::vector<int> &v); // checks if exists the chromosome
};

#endif
