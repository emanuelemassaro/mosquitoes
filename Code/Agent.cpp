#include "stdafx.h"
#include <cassert>
#include <mpi.h>
#include <random>
#include <iomanip>
#include <vector>
#include <iostream>
#include "Agent.h"


#define DEFAULT_ITERATIONS_COUNT 5
#define LOOPS_IN_ITERATION 5000
#define TASK_TAG 1024
#define irand(N) ((int) (drand48()*(N)))
#define RAND_0_1 (double)real_distribution(mt_engine)

using namespace std;

// Define Nodes and Agents

Agent *initialize_agents(int N, int V)
{
    Agent  *A = new Agent[N];
	char fileN[1024] =  "networkSimulations/realNetwork.dat";
	FILE *f = fopen(fileN, "r"); // Binary File Home Work
	int k = 0;
	int v = 0;
	while (!feof(f))
	{
		int i, j;
		fscanf(f, "%d %d", &i, &j);
		A[k].home = i;
		A[k].work = j;
		k++;
	}
	return(A); 
}
