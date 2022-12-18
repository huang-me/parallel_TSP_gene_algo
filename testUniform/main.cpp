#include "test.h"

#include <unistd.h>
#include <stdlib.h>

int threadpool::executed, threadpool::epochs, threadpool::num_nodes = 100;
vector<int> threadpool::freq;
vector<int> threadpool::freq_rand;
state *threadpool::st = new state;

int main(int argc, char *argv[]) {
	int num = 10000, thread_num = 4;
	char opt;
	while ((opt = getopt(argc, argv, "t:n:c:")) != -1) {
		switch (opt) {
			case 'n':
				num = atoi(optarg);
				break;
			case 't':
				thread_num = atoi(optarg);
				break;
			case 'c':
				threadpool::num_nodes = atoi(optarg);
				break;
			case '?':
				return 1;
		}
	}
	threadpool tp(thread_num, num);
	tp.showResult();
	return 0;
}
