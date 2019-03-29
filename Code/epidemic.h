#include "stdafx.h"
#include <mpi.h>
#include <cassert>
#include <random>
#include <iomanip>
#include <vector>
#include <iostream>

#define RAND_0_1_ (double) real_distribution1(mt_engine1)
using namespace std;
mt19937 mt_engine1;

uniform_real_distribution<double> real_distribution1;
void perform_process(long total_iterations, int current_rank, int world_size, int Iyear, int Inet);

using namespace std;
void epidemic(Node *K, Agent *A, int N, int V, int day,
               double rateSE, double rateEI, double rateIR,
               double biteRate,
               double rateSEm, double rateEIm,
               double rateMUv, double rateASm, double rateMUa, double rateVA,
               double xv)

/*
 * K = Node of network. In each node we have humans and mosquito in the different states of the epidemic
 * A = Agents in the model. Agents have two favorite cells: home and work locations
 * N = number of humans
 * V = number of cells
 * T = temperature
 * limitK = max number of mosquito in each cell
	*/
{
    vector<int> tmp;
    double p;
    for (int i = 0; i < V; i++)
    {
        tmp = K[i].home;
        if (day == 1) tmp = K[i].work;  // if day tmp = work else tmp = home
        
        int Amosquito = K[i].Amosquito;
        int Imosquito = K[i].Imosquito;
        int Smosquito = K[i].Smosquito;
        int Emosquito = K[i].Emosquito;
        int mosquito =  K[i].Emosquito + K[i].Imosquito + K[i].Smosquito;
		
        double rsem = (double) rateSEm * (biteRate) * Smosquito * K[i].i / tmp.size();
        const double reim = (double) rateEIm;
        

 
        // mosquito population (according to: Lourenço, J., & Recker, M. (2014).
        // The 2012 Madeira Dengue Outbreak: Epidemiological Determinants and Future Epidemic Potential.
        // PLoS Neglected Tropical Diseases, 8(8).
        //		http://doi.org/10.1371/journal.pntd.0003083 )
        // 1. calculate mosquitos that die (from S,E and I)
        //rateMUv = 0.09; //mortality rate 
        binomial_distribution<int> distSd(Smosquito, rateMUv);
        binomial_distribution<int> distEd(Emosquito, rateMUv);
        binomial_distribution<int> distId(Imosquito, rateMUv);
        
        int tmp0 = distSd(mt_engine1);
        K[i].Smosquito -= tmp0;
        tmp0 = distEd(mt_engine1);
        K[i].Emosquito -= tmp0;
        tmp0 = distId(mt_engine1);
        K[i].Imosquito -= tmp0;
        
        

        // 2. calculate transitions according to the rates
        if (rsem > 1){
			rsem = 0.99;
		}
        binomial_distribution<int> distSE(K[i].Smosquito, rsem);        
        tmp0 = distSE(mt_engine1); // mosquitos that survived and transition S->E
        
        binomial_distribution<int> distEI(K[i].Emosquito, reim);
        int tmp1 = distEI(mt_engine1); // mosquitos that survived and transision E->I 
        K[i].Smosquito -= tmp0; // transition
        K[i].Emosquito += tmp0; // S->E
        K[i].Emosquito -= tmp1; // transition
        K[i].Imosquito += tmp1; // E->I
        
        /* -- this replaced by Amosquitos
         binomial_distribution<int> distIR(Imosquito, rirm);
         tmp0 = distIR(mt_engine);
         K[i].Imosquito -= tmp0;
         K[i].Smosquito += tmp0;
         */
        
        // 3. Advance aquatic mosquitos
        // Eq. (6)
        // 3.1. aquatic mosquitos that die
        binomial_distribution<int> distAd(Amosquito, rateMUa);

        tmp0 = distAd(mt_engine1);
        K[i].Amosquito -= tmp0;
        // 2. transision A->S
        binomial_distribution<int> distAS(K[i].Amosquito, rateASm);
        tmp0 = distAS(mt_engine1);
        K[i].Amosquito -= tmp0;
        K[i].Smosquito += tmp0;
        const double wmax = 1000; // maximum water level
        
        
        // 3.2 new aquatic mosquitos: Poisson process with mosquitos * rateVA total rate
        // 3.2.1 calculate "local" K limit value -> include green area and population
        double limitK2 =  xv * (double)( K[i].work.size() + K[i].home.size() ) / 2.0;
        
        
        // factor for the daily rain should be here
        // !! TODO: scale limitK with green area and rainfall !!
  
        poisson_distribution<int> distVA( ((double) mosquito) * rateVA * ( 1 - ((double)Amosquito) / limitK2 ) ); //
        tmp0 = distVA(mt_engine1);
        K[i].Amosquito += tmp0;
        if(K[i].Amosquito > limitK2) K[i].Amosquito = limitK2;
        K[i].mosquito = K[i].Emosquito + K[i].Imosquito + K[i].Smosquito;
 
		const double rse = (double) rateSE * Imosquito * K[i].s * biteRate / tmp.size();
        const double rei = (double) rateEI;
        const double rir = (double) rateIR;
        
        
        for (int j = 0; j < tmp.size(); j++)
        {
            int tmpNode = tmp[j];
            switch (A[tmpNode].status)
            {
                case 0: //suseptible ---> Exposed
                    p = RAND_0_1_;
                    if (p < rse)
                    {
                        A[tmpNode].status = 1;
                        K[i].s--;
                        K[i].e++;
                    }
                    break;
                    
                case 1: //exposed ---> Infected
                    p = RAND_0_1_;
                    if (p < rei)
                    {
                        A[tmpNode].status = 2;
                        K[i].e--;
                        K[i].i++;
                    }
                    break;
                    
                case 2: //infected ----> Recovered
                    p = RAND_0_1_;
                    if (p < rir)
                    {
                        A[tmpNode].status = 3;
                        K[i].i--;
                        K[i].r++;
                    }
                    break;
            }
        }
        
    }
}
