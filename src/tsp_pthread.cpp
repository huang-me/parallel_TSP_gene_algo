#include "tsp_pthread.h"

using namespace std;

state *rand_st = new state;

// constructor of Genetic
Genetic_thread::Genetic_thread(Graph *g, int size_population, int generations,
                               int mutation_rate, int thread_num,
                               bool show_population) {
  if (size_population < 1) // checks if size of population is less than 1
  {
    cout << "Error: size_population < 1\n";
    exit(1);
  } else if (mutation_rate < 0 ||
             mutation_rate > 100) // checks if mutation rate is less than 0
  {
    cout << "Error: mutation_rate must be >= 0 and <= 100\n";
    exit(1);
  }
  this->graph = g;
  this->size_population = size_population;
  this->real_size_population = 0;
  this->generations = generations;
  this->mutation_rate = mutation_rate;
  this->show_population = show_population;
  this->thread_cnt = thread_num;
  this->bestCost = INT_MAX;
}

// checks if is a valid solution, then return total cost of path else return -1
int Genetic_thread::isValidSolution(vector<int> &solution) {
  int total_cost = 0;
  set<int> set_solution;

  // checks if not contains elements repeated
  for (int i = 0; i < graph->V; i++)
    set_solution.insert(solution[i]);

  if (set_solution.size() != (unsigned)graph->V)
    return -1;

  // checks if connections are valid
  for (int i = 0; i < graph->V; i++) {
    if (i + 1 < graph->V) {
      int cost = graph->existsEdge(solution[i], solution[i + 1]);

      // checks if exists connection
      if (cost == -1)
        return -1;
      else
        total_cost += cost;
    } else {
      int cost = graph->existsEdge(solution[i], solution[0]);

      // checks if exists connection
      if (cost == -1)
        return -1;
      else
        total_cost += cost;
      break;
    }
  }
  return total_cost;
}

bool Genetic_thread::existsChromosome(const vector<int> &v) {
  // checks if exists in the population
  for (vector<pair<vector<int>, int>>::iterator it = population.begin();
       it != population.end(); ++it) {
    const vector<int> &vec = (*it).first;       // gets the vector
    if (equal(v.begin(), v.end(), vec.begin())) // compares vectors
      return true;
  }
  return false;
}

bool Genetic_thread::_findInitial(vector<int> &arr, int curr, vector<bool> &visited, int &visitedNum) {
	arr.push_back(curr);
	visited[curr] = true;
	visitedNum++;
	if(visitedNum == graph->V)
		return true;
	for(int i=0; i<graph->V; ++i) {
		if(visited[i]) continue;
		if(graph->existsEdge(curr, i) != -1) {
			if(_findInitial(arr, i, visited, visitedNum))
				return true;
		}
	}
	visited[curr] = false;
	visitedNum--;
	arr.pop_back();
	return false;
}

void Genetic_thread::initialPopulation() // generates the initial population
{
  vector<int> parent;

  // creates the parent
  vector<bool> visited(graph->V, 0);
  int visitedNum = 0;
  _findInitial(parent, graph->initial_vertex, visited, visitedNum);

  int total_cost = isValidSolution(parent);

  if (total_cost != -1) {
    // checks if the parent is valid
    population.push_back(
        make_pair(parent, total_cost)); // inserts in the population
    real_size_population++;             // increments real_size_population
  }

  // checks if real_size_population is 0
  if (real_size_population == 0)
    cout << "\nEmpty initial population ;( Try again runs the algorithm...";
  else
    sort(population.begin(), population.end(), sort_pred()); // sort population
}

void Genetic_thread::showPopulation() {
  cout << "\nShowing solutions...\n\n";
  for (vector<pair<vector<int>, int>>::iterator it = population.begin();
       it != population.end(); ++it) {
    const vector<int> &vec = (*it).first; // gets the vector

    for (int i = 0; i < graph->V; i++)
      cout << vec[i] << " ";
    cout << graph->initial_vertex;
    cout << " | Cost: " << (*it).second << "\n\n";
  }
  cout << "\nPopulation size: " << real_size_population << endl;
}

// inserts in the vector using binary search
void Genetic_thread::insertBinarySearch(vector<int> &child, int total_cost) {
  int imin = 0;
  int imax = real_size_population - 1;

  while (imax >= imin) {
    int imid = imin + (imax - imin) / 2;

    if (total_cost == population[imid].second) {
      population.insert(population.begin() + imid,
                        make_pair(child, total_cost));
      real_size_population++;
      return;
    } else if (total_cost > population[imid].second)
      imin = imid + 1;
    else
      imax = imid - 1;
  }
  population.insert(population.begin() + imin, make_pair(child, total_cost));
  real_size_population++;
}

/*
        Makes the crossover
        This crossover selects two random points
        These points generates substrings in both parents
        The substring inverted of parent1 is placed in parent2 and
        the substring inverted of parent2 is placed in parent1

        Example:
                parent1: 1 2 3 4 5
                parent2: 1 2 4 5 3

                substring in parent1: 2 3 4
                substring in parent2: 2 4 5

                substring inverted in parent1: 4 3 2
                substring inverted in parent2: 5 4 2

                child1: 1 5 4 2 5
                child2: 1 4 3 2 3

                Children are invalids: 5 appears 2x in child1 and 3 appears 2x
   in child2 Solution: map of genes that checks if genes are not used
*/
void Genetic_thread::crossOver(vector<int> parent1, vector<int> parent2) {
  vector<int> child1, child2;

  // map of genes, checks if already are selected
  map<int, int> genes1, genes2;

  for (int i = 0; i < graph->V; i++) {
    // initially the genes not are used
    genes1[parent1[i]] = 0;
    genes2[parent2[i]] = 0;
  }

  // generates random points
  int point1 = rand_st->myrand() % (graph->V - 1) + 1;
  int point2 = rand_st->myrand() % (graph->V - point1) + point1;

  // adjusts the points if they are equal
  if (point1 == point2) {
    if (point1 - 1 > 1)
      point1--;
    else if (point2 + 1 < graph->V)
      point2++;
    else {
      // point1 or point2 ?? random...
      int point = rand_st->myrand() % 10 + 1; // number in the range 1 to 10
      if (point <= 5)
        point1--;
      else
        point2++;
    }
  }

  // generates childs

  // until point1, child1 receives genes of the parent1
  // and child2 receives genes of the parent2
  for (int i = 0; i < point1; i++) {
    // adds genes
    child1.push_back(parent1[i]);
    child2.push_back(parent2[i]);
    // marks genes
    genes1[parent1[i]] = 1;
    genes2[parent2[i]] = 1;
  }

  // marks remaining genes
  for (int i = point2 + 1; i < graph->V; i++) {
    genes1[parent1[i]] = 1;
    genes2[parent2[i]] = 1;
  }

  // here is the substring inverted
  // child1 receives genes of the parent2 and
  // child2 receives genes of the parent1
  for (int i = point2; i >= point1; i--) {
    if (genes1[parent2[i]] == 0) // if the gene is not used
    {
      child1.push_back(parent2[i]);
      genes1[parent2[i]] = 1; // marks the gene
    } else {
      // if the gene already is used, chooses gene that is not used
      for (map<int, int>::iterator it = genes1.begin(); it != genes1.end();
           ++it) {
        if (it->second == 0) // checks if is not used
        {
          child1.push_back(it->first);
          genes1[it->first] = 1; // marks as used
          break;                 // left the loop
        }
      }
    }

    if (genes2[parent1[i]] == 0) // if the gene is not used
    {
      child2.push_back(parent1[i]);
      genes2[parent1[i]] = 1; // marks the gene
    } else {
      // if the gene already is used, chooses gene that is not used
      for (map<int, int>::iterator it = genes2.begin(); it != genes2.end();
           ++it) {
        if (it->second == 0) // checks if is not used
        {
          child2.push_back(it->first);
          genes2[it->first] = 1; // marks as used
          break;                 // left the loop
        }
      }
    }
  }

  // ramaining genes: child1 receives genes of the parent1
  // and child2 receives genes of the parent2
  for (int i = point2 + 1; i < graph->V; i++) {
    child1.push_back(parent1[i]);
    child2.push_back(parent2[i]);
  }

  // mutation
  int mutation = rand_st->myrand() % 100 + 1; // random number in [1,100]
  if (mutation <= mutation_rate) // checks if the random number <= mutation rate
  {
    // makes a mutation: change of two genes

    int index_gene1, index_gene2;
    index_gene1 = rand_st->myrand() % (graph->V - 1) + 1;
    index_gene2 = rand_st->myrand() % (graph->V - 1) + 1;

    // makes for child1
    int aux = child1[index_gene1];
    child1[index_gene1] = child1[index_gene2];
    child1[index_gene2] = aux;

    // makes for child2
    aux = child2[index_gene1];
    child2[index_gene1] = child2[index_gene2];
    child2[index_gene2] = aux;
  }

  int total_cost_child1 = isValidSolution(child1);
  int total_cost_child2 = isValidSolution(child2);

  m.lock();
  // checks if is a valid solution and not exists in the population
  if (total_cost_child1 != -1 && !existsChromosome(child1)) {
    // add child in the population
    insertBinarySearch(child1,
                       total_cost_child1); // uses binary search to insert
  }
  m.unlock();

  m.lock();
  // checks again...
  if (total_cost_child2 != -1 && !existsChromosome(child2)) {
    // add child in the population
    insertBinarySearch(child2,
                       total_cost_child2); // uses binary search to insert
  }
  m.unlock();
}

// runs the genetic algorithm
vector<int> Genetic_thread::run() {
  initialPopulation(); // gets initial population

  if (real_size_population == 0)
    return {};

  vector<thread> threads;
  for (int i = 0; i < generations; i += thread_cnt) {
    // TODO: single run
    for (int j = 0; j < thread_cnt; ++j)
      threads.push_back(thread(&Genetic_thread::single_run, this));
    for (int j = 0; j < thread_cnt; ++j)
      threads[j].join();
    threads.clear();
    removeBadGenes();
  }

  if (show_population == true)
    showPopulation(); // shows the population

  // cout << "\nBest solution: ";
  vector<int> vec = population[0].first;
  // for (int i = 0; i < graph->V; i++)
  //   cout << vec[i] << " ";
  // cout << graph->initial_vertex;
  // cout << " | Cost: " << population[0].second << endl;
  bestCost = population[0].second;
  return vec;
}

int Genetic_thread::getCostBestSolution() {
  if (real_size_population > 0)
    return population[0].second;
  return -1;
}

void Genetic_thread::removeBadGenes() {
  while (real_size_population > size_population) {
    population.pop_back();
    real_size_population--;
  }
}

void Genetic_thread::single_run(void) {
  // TODO: solve population's race condition
  int pop_sz = __sync_fetch_and_add(&real_size_population, 0);

  /* selects two parents (if exists) who will participate
                                  of the reproduction process */
  if (pop_sz >= 2) {
    if (pop_sz == 2) {
      // applying crossover in the parents
      crossOver(population[0].first, population[1].first);
    } else {
      // real_size_population > 2

      int parent1, parent2;

      do {
        // select two random parents
        parent1 = rand_st->myrand() % pop_sz;
        parent2 = rand_st->myrand() % pop_sz;
      } while (parent1 == parent2);

      // applying crossover in the two parents
      crossOver(population[parent1].first, population[parent2].first);
    }
  } else {
	// population contains only 1 parent

    // applying crossover in the parent
    crossOver(population[0].first, population[0].first);
  }
  return;
}

int Genetic_thread::getBestCost() {
	return bestCost;
}

PYBIND11_MODULE(_Genetic, t)
{
    t.doc() = "Multi-thread TSP solver with genetic algorithm";

	pybind11::class_<Genetic_thread>(t, "Genetic_thread")
		.def(pybind11::init<Graph*, int, int, int, int, bool>())
		.def("run", &Genetic_thread::run)
		.def("getBestCost", &Genetic_thread::getBestCost);
}
