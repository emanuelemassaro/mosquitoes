#include "stdafx.h"
#include <mpi.h>
#include <cassert>
#include <random>
#include <iomanip>
#include <vector>
#include <iostream>



void returnSize(int& V, int& N)
{
	char fileN[1024] =  "networkSimulations/realNetwork.dat";
	FILE *f = fopen(fileN, "r");
	while (!feof(f))
	{
		N += 1;
		int i, j;
		fscanf(f, "%d %d", &i, &j);
		if (i > V)
			V = i;
		if (j > V)
			V = j;
	}
	V++;
	fclose(f);
}
