#ifndef INITIALIZE_AGENTS_H
#define INITIALIZE_AGENTS_H

#include "stdafx.h"
#include <mpi.h>
#include <cassert>
#include <random>
#include <iomanip>
#include <vector>
#include <iostream>
#include "Agent.h"

using namespace std;


// Define Nodes and Agents
Agent *initialize_agents(int N, int V, int net, int Inet)
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
#endif
