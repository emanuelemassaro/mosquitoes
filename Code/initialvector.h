#include "stdafx.h"
#include <mpi.h>
#include <cassert>
#include <random>
#include <iomanip>
#include <vector>
#include <iostream>
#include "Node.h"
#include "Agent.h"



void initialvector(double xv, Node *K, Agent *A)
{
	char fileG[1024];
	sprintf(fileG, "Initial/aquatic%d_%.2f_%.1f.dat", 2013, xv, 10.0);
	FILE *f0 = fopen(fileG, "r"); // Binary File Home Work
	// Read aquatic mosquito from the previous year
	while (!feof(f0))
	{
		int temp;
		int cell;
		fscanf(f0, "%d %d", &cell, &temp);
		K[cell].Amosquito = temp;
	}
	fclose(f0);


	sprintf(fileG, "Initial/adult%d_%.2f%.1f.dat", 2013, xv, 10.0);
	f0 = fopen(fileG, "r"); // Binary File Home Work
	// Read Adult mosquito from the previous year
	int sumMosquito = 0;
	while (!feof(f0))
	{
		int temp;
		int cell;
		fscanf(f0, "%d %d", &cell, &temp);
		K[cell].Smosquito = temp;
		K[cell].mosquito  = temp;
		sumMosquito += temp;
		
	}
	fclose(f0);
	
}
			
