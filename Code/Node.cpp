#include "stdafx.h"
#include <mpi.h>
#include <cassert>
#include <random>
#include <iomanip>
#include <vector>
#include <iostream>
#include "Agent.h"
#include "Node.h"


Node *initialize_nodes(int N, int V)
{
    Node *K = new Node[V];
	Agent  *A = new Agent[N];
	char fileN[1024] = "networkSimulations/realNetwork.dat";
	FILE *f = fopen(fileN, "r"); // Binary File Home Work
	int k = 0;
	int v = 0;
	while (!feof(f))
	{
		int i, j;
		fscanf(f, "%d %d", &i, &j);
		K[i].work.push_back(k);
		K[j].home.push_back(k);
		k++;
	}
    return(K);
}

