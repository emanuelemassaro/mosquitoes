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
    if(Inet == 0){
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
    } else if (Inet == 1){
        char fileN[1024];
        sprintf(fileN, "networkSimulations/sameDistNet.dat");
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
    } else if (Inet == 2){
        Agent  *A = new Agent[N];
        char fileN[1024] =  "networkSimulations/realNetwork.dat";
        FILE *f = fopen(fileN, "r"); // Binary File Home Work
        int k = 0;
        int v = 0;
        while (!feof(f))
        {
            int i, j;
            fscanf(f, "%d %d", &i, &j);
            A[k].home = irand(V);
            A[k].work = j;
            k++;
        }
        return(A);
    } else if (Inet == 3){
        Agent  *A = new Agent[N];
        char fileN[1024] =  "networkSimulations/levyNetwork.dat";
        FILE *f = fopen(fileN, "r"); // Binary File Home Work
        int k = 0;
        int v = 0;
        while (!feof(f))
        while (!feof(f))
        {
            int i, j;
            fscanf(f, "%d %d", &i, &j);
            A[k].home = i;
            A[k].work = j;
            k++;
        }
        return(A);
    } else if (Inet == 4){
        Agent  *A = new Agent[N];
        char fileN[1024] =  "networkSimulations/radialNetwork.dat";
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
    
}
#endif
