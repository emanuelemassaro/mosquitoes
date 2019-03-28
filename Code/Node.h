// File Node.h

#ifndef NODE_H
#define NODE_H

#include "stdafx.h"
#include <mpi.h>
#include <cassert>
#include <random>
#include <iomanip>
#include <vector>
#include <iostream>

using namespace std;
struct Node
{
    int s, oldS; // susceptible
    int e, oldE; // exposed
    int i, oldI; // infected
    int r, oldR; // recovered
    int n, oldN;
    int mosquito; // Smosquito + Emosquito + Imosquito
    int Amosquito; //aquatic mosquito
    int Smosquito;
    int Emosquito;
    int Imosquito;
    
    vector<int> work;
    vector<int> home;    // total work, total home
    double waterLevel;
    double greenArea;
    
    // breeding sites scaling factor (between 0 and 1)
    double B;
    
    Node()
    {
        s = oldS = 0;
        i = oldI = 0;
        r = oldR = 0;
        e = oldE = 0;
        Amosquito = 0;
        Emosquito = 0;
        Imosquito = 0;
        Smosquito = 0;
        mosquito  = 0;
        mosquito  = 0;
        waterLevel  = 1;
        greenArea = 1;
        B = 0.5; // default value
    }
    
    inline void prepareOldStates()
    {
        oldS = s;
        oldI = i;
        oldR = r;
        oldE = e;
        oldN = n;
    }
    
};
Node *initialize_nodes(int N, int V);

#endif
