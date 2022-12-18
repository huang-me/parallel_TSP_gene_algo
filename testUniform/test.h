#ifndef TEST_H
#define TEST_H

#include <queue>
#include <vector>
#include <cstdio>
#include <random>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

using namespace std;

class state {
	public:
		uint32_t s = time(NULL);
		uint32_t rand() {
			uint32_t x, old;
			do {
				x = old = s;
				x ^= x << 13;
				x ^= x >> 17;
				x ^= x << 5;
			} while(!__sync_bool_compare_and_swap(&s, old, x));
			return x;
		}
};

class threadpool {
	public:
		vector<pthread_t> threads;
		static vector<int> freq;
		static vector<int> freq_rand;
		static int executed, epochs, num_nodes;
		static state *st;
		threadpool(int n, int e) {
			executed = 0;
			epochs = e;
			threads.resize(n);
			freq.resize(num_nodes, 0);
			freq_rand.resize(num_nodes, 0);
			srand(time(NULL));

			for(int i=0; i<n; ++i) {
				pthread_create(&threads[i], NULL, &threadpool::run, NULL);
			}
			for(int i=0; i<n; ++i)
				pthread_join(threads[i], NULL);
		}
		static void* run(void*) {
			do {
				int id = __sync_add_and_fetch(&executed, 1);
				if(id > epochs) return NULL;
				unsigned tmp = st->rand();
				__sync_fetch_and_add(&freq[tmp % num_nodes], 1);
				__sync_fetch_and_add(&freq_rand[rand() % num_nodes], 1);
			} while(1);
			return NULL;
		}
		void showResult() {
			for(int i=0; i<num_nodes; ++i) {
				printf("%d\n", freq[i]);
				fprintf(stderr, "%d\n", freq_rand[i]);
			}
		}
};

#endif
