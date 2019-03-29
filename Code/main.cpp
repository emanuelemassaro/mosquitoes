// MPI_Mosquito Disease Model
//
// By Emanuele Massaro (2019)

#include "stdafx.h"
#include <cassert>
#include <random>
#include <iomanip>
#include <vector>
#include <iostream>
#include <string>
#include "returnSize.h"
#include "Node.h"
#include "Agent.h"
#include "epidemic.h"
#include "travel.h"
#include "readtempall.h"
#include "initialexposed.h"
#include "initialvector.h"

#define DEFAULT_ITERATIONS_COUNT 5
#define LOOPS_IN_ITERATION 5000
#define TASK_TAG 1024
#define irand(N) ((int) (drand48()*(N)))
#define RAND_0_1 (double)real_distribution(mt_engine)

using namespace std;
// Random stuff
mt19937 mt_engine;
uniform_real_distribution<double> real_distribution;
void perform_process(long total_iterations, int current_rank, int world_size, double xv_init, double br_init, 
double rateSE, double rateEI, double rateIR, double rateSEm, double rateEIm, double rateMUv, double rateMUa, double rateASm, double rateVA);
char fN[TASK_TAG];
bool nompi = false;


void perform(long total_iterations, int current_rank, int world_size, double xv_init, double br_init,
double rateSE, double rateEI, double rateIR, double rateSEm, double rateEIm, double rateMUv, double rateMUa, double rateASm, double rateVA)
{
    int V = 0;
	int N = 0;
	srand(time(NULL) + current_rank);
    
	returnSize(V, N); // Get size of the Network and number of Agents
	//V=V-1;
	
	
    // Read Temperature  
    vector<double> temperature;
    temperature = readtempall(); 
    int size = temperature.size();
    
    // Bite rate and mosquitoes per humans
    vector<double> XV({xv_init});  // number of mosquito per human  ---> P1
    vector<double> BR({br_init});  // biterate (per day!) ----> P2
    
    
    int total_runs = total_iterations*XV.size()*BR.size();
    int days = 364;
    
    for(int i1 = current_rank; i1 < total_runs; i1 += world_size) {
        int i = i1 / (XV.size()*BR.size());
        int i2 = i1 % (XV.size()*BR.size());
        int p1 = i2 / BR.size();
        int p2 = i2 % (BR.size());
        double biteRate = BR[p2];
        double xv = XV[p1];    
        char fileOutput[1024];
        sprintf(fileOutput, "results/run_%d_%lf_%lf.csv", i, xv, biteRate);

        
		FILE * file;
		file = fopen(fileOutput, "r");
		if (file){
			printf("The file already results/run_%d_%lf_%lf.csv already exists \n", i, xv, biteRate);
			fclose(file);
		}else{
            Node  *K = initialize_nodes(N, V); // Initialize Nodes
            Agent *A = initialize_agents(N, V); // Initialize Agents
           

            cout << "Node #" << current_rank << ", iteration " << i << ", p1 " << p1 << " (xv: " << XV[p1] << "), p2 " << p2 << " (br: " << BR[p2] << ")\n";
            int year = 2013;
            initialexposed(year, K, A);  // read the initial number of infected for that year
            initialvector(xv, K, A);  // read the initial number of mosquito for that year
            
            FILE *f = fopen(fileOutput, "w");
            fprintf(f, "Node S E I R A M IM EM S Time\n");
            
            FILE *f1;
            char fileI[1024];
            
            int count = 0; 
            int t = 0;
            while (t/2 < days){
                const double T0 = temperature[t/2];  
                int day = 1;
                if (t % 2 == 0) day = 0; // home
                
                epidemic(K, A, N, V, day, rateSE/2, rateEI/2, rateIR/2, biteRate,  rateSEm/2, rateEIm/2, rateMUv/2, rateASm/2, rateMUa/2, rateVA/2, xv);
                travel(K, A, V, N, day);
                
                t += 1;
                count+=1;
                int IM = 0;
                if (t % 2 == 0){
                    for (int v = 0; v < V; v++)   
                    { 	
                        fprintf(f, "%d %d %d %d %d %d %d %d %d %d %d\n", v, K[v].s, K[v].e, K[v].i, K[v].r, K[v].Amosquito, K[v].mosquito, K[v].Imosquito, K[v].Emosquito, K[v].Smosquito, t/2);
                        IM += K[v].i;
                    }
                    count=0;
                }
            }

            fclose(f);
            delete[]A;
            delete[]K;
        }
    
    }
    
}
int main(int argc, char* argv[])
{
    
    long total_iterations = 0;
    double xv_init, br_init, rateSE, rateEI, rateIR, rateSEm, rateEIm, rateMUv, rateMUa, rateASm, rateVA;
    
    total_iterations = strtol(argv[1], NULL, 10);
    
    xv_init = atof(argv[2]);
    br_init = atof(argv[3]);
    
    rateSE = atof(argv[4]);
    rateEI = atof(argv[5]);
    rateIR = atof(argv[6]);
    rateSEm = atof(argv[7]);
    rateEIm = atof(argv[8]);
    rateMUv = atof(argv[9]);
    rateMUa = atof(argv[10]);
    rateASm = atof(argv[11]);
    rateVA = atof(argv[12]);
    
    
    if (total_iterations == 0)
    {
        total_iterations = DEFAULT_ITERATIONS_COUNT;
    }
    if(total_iterations == -1) { // can be used to run without mpi
        total_iterations = DEFAULT_ITERATIONS_COUNT;
        nompi = true;
    }
    
    // Randomize
    
    random_device rd;
    mt_engine.seed(rd());
    int current_rank = 0;
    double tic = 0;
    double toc = 0;
    int world_size = 1;
    
    if(!nompi) {
        // Initialize MPI and basic variables
        MPI_Init(&argc, &argv);
        
        tic = MPI_Wtime();
        
        MPI_Comm_size(MPI_COMM_WORLD, &world_size);
        
        MPI_Comm_rank(MPI_COMM_WORLD, &current_rank);
    }
    
    mt_engine.seed(time(0) + current_rank);
    

    char string2[1024];
    sprintf(string2, "logerror/error_%d.txt", current_rank);
    
    
    freopen(string2, "w", stderr );
    // Run the main process
    perform(total_iterations, current_rank, world_size, xv_init, br_init, rateSE, rateEI, rateIR, rateSEm, rateEIm, rateMUv, rateMUa, rateASm, rateVA);
    
    
    // Finish and produce summary
    if(!nompi) {
        toc = MPI_Wtime();
        MPI_Finalize();
        
        if (current_rank == 0)
        {
            cout << "--- Operation took " << (toc - tic) << " seconds ---" << endl;
            system("pause");
        }
        
    }
    
    cerr << "Error message" << endl;
	
    return 0;
}


