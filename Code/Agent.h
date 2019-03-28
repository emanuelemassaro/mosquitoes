// File agent.h

#ifndef AGENT_H
#define AGENT_H

#include "stdafx.h"
#include <mpi.h>
#include <cassert>
#include <random>
#include <iomanip>
#include <vector>
#include <iostream>
// Define Nodes and Agents



struct Agent
{
    int home, work; // Locations
    int status; // S=0; E=1; I=2; R=3    
    
    Agent()
    {
        status = 0;
    }
    
    
};
Agent *initialize_agents(int N, int V);

#endif
