#!/usr/bin/python3

import _graph
import _Genetic

def test_correctness(arr, nodenum):
	if(len(arr) != nodenum):
		return False
	exist = [0] * nodenum
	for a in arr:
		if(exist[a]==1):
			return False
		exist[a] = 1
	print(arr)
	return True

if __name__ == "__main__":
	nodenum = 50
	numthread = 4
	g = _graph.Graph(nodenum, 0, True, "pytest.txt")
	genetic = _Genetic.Genetic_thread(g, 10, 10000, 5, numthread, False)
	res = genetic.run();
	correctness = test_correctness(res, nodenum)
	assert correctness is True
	print("Best Cost: {}".format(genetic.getBestCost()))
	print("Done")
