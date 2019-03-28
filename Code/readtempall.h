#include "stdafx.h"
#include <mpi.h>
#include <cassert>
#include <random>
#include <iomanip>
#include <vector>
#include <iostream>

std::vector <double> readtempall()
{// Read Temperature
	vector<double> temperature;
	char fileN[1024];
	sprintf(fileN, "weatherData/temp/tempAll1.dat");
	FILE *f0 = fopen(fileN, "r"); // Binary File Home Work

	while (!feof(f0))
	{
    		double temp;
    		fscanf(f0, "%lf", &temp);
    		temperature.push_back(temp);
	}
	fclose(f0);
	return temperature;
}
