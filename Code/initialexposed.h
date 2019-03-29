#include "stdafx.h"
#include <mpi.h>
#include <cassert>
#include <random>
#include <iomanip>
#include <vector>
#include <iostream>
#include "Node.h"
#include "Agent.h"

void initialexposed(int year, Node *K, Agent *A)
{
	
	// Load Initial Exposed
	char fileI[1024];
	int yeah = year;
	sprintf(fileI, "Initial/initial.dat");
	FILE *f1 = fopen(fileI, "r"); // Binary File Home Work
	
	int cases = 0;
	int cell = 0;
	int initial_infected = 0;
	int infected = 0;
	while (!feof(f1))
	{
		fscanf(f1,"%d %d", &cell, &cases);
		for (int k = 0; k < cases; k++)
		{
			int agent = K[cell].home[k];
			A[agent].status  = 1;
			initial_infected += 1;
			K[cell].e++;
		}
	}
	fclose(f1);
	
}
