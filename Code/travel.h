#include "stdafx.h"
#include <mpi.h>
#include <cassert>
#include <random>
#include <iomanip>
#include <vector>
#include <iostream>

void travel(Node *K, Agent *A, int V, int N, int day)
{
    if (day) day = 0;
    else day = 1;
    
    for (int i = 0; i < V; i++)
    {
        K[i].e = 0;
        K[i].s = 0;
        K[i].i = 0;
        K[i].r = 0;
    }
    
    for (int i = 0; i < N; i++)
    {
        int node = A[i].home;
        if (day) node = A[i].work;
        
        switch (A[i].status)
        {
            case 0:
                K[node].s++;
                break;
            case 1:
                K[node].e++;
                break;
            case 2:
                K[node].i++;
                break;
            case 3:
                K[node].r++;
                break;
        }
    }
}
