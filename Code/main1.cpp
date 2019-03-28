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
void perform_process(long total_iterations, int current_rank, int world_size);
char fN[TASK_TAG];
bool nompi = false;


void perform(long total_iterations, int current_rank, int world_size)
{
    int V = 0;
	int N = 0;
    
	returnSize(V, N); // Get size of the Network and number of Agents
	V+=1;

    // Read Temperature  
    vector<double> temperature;
    temperature = readtempall(); 
    int size = temperature.size();

    
    // Define paremeters
    const double deltaT = 15;
    const double deltaT1 = 10;
    const double R = 8.314;
    const double rateEI = 0.5; 
    const double rateIR = 0.143; 
    
    // Bite rate and mosquitoes per humans
    vector<double> XV({0.004,0.005,0.006,0.007,0.008,0.009,0.01});  // number of mosquito per human  ---> P1
    vector<double> BR({0.1,0.12,0.14,0.16,0.18,0.2,0.22,0.24,0.26,0.28,0.3,0.32,0.34,0.36,0.38,0.4});  // biterate (per day!) ----> P2
    
    
    
    int total_runs = total_iterations*XV.size()*BR.size();
    int days = 729;
    
    for(int i1 = current_rank; i1 < total_runs; i1 += world_size) {
        int i = i1 / (XV.size()*BR.size());
        int i2 = i1 % (XV.size()*BR.size());
        int p1 = i2 / BR.size();
        int p2 = i2 % (BR.size());
        double biteRate = BR[p2];
        double xv = XV[p1];    
        char fileOutput[1024];
        sprintf(fileOutput, "results/run_%d_%lf_%lf_%.1f_%.1f_real.csv", i, xv, biteRate, deltaT, deltaT1);

        
		FILE * file;
		file = fopen(fileOutput, "r");
		if (file){
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
            
            int count = 0; //print every 7 days
            int t = 0;
            while (t/2 < days){
                const double T0 = temperature[t/2];
                const double T  = T0 - deltaT;  // Infection
                const double T1 = T0 - deltaT1; // Mosquito
                const double Tk = T + 273.15;
                const double VP = 6.11 * pow(10, (7.5*T)/(237.3 + T)) / 10;
                double RHF;
                if (VP >= 30)
                {
                    RHF = 0.5;
                }else
                {
                    RHF = 1.2 - 0.2 * VP;
                }
                // Define rate of transitions
                const double rateSE  = 0.0729*T- 0.97; // H: S->E
                const double rateSEm = 1.004e-3 * T * (T - 12.286) * pow((32.461-T),0.5); // V: S->E
                const double nominatore = (3.3589e-3 * Tk)/298 * exp( (1500/R) * (1/298-1/Tk));
                const double denominatore = 1 + exp( ( 6.203e21 )/R * (1/(-2.176e30)) - 1/Tk );
                const double rateEIm = ( nominatore ) / ( denominatore );
                const double rateMUv = RHF * (0.8692 - 0.1599*T + 0.01116 * pow(T,2) - 3.408e-4 * pow(T,3) + 3.809e-6 * pow(T,4));
                const double rateMUa = 2.13 - 0.3787*T1 + 0.02457 * pow(T1,2)  - 6.778e-4 * pow(T1,3) + 6.794e-6 * pow(T1,4);
                const double rateASm = 0.131 - 0.05723*T1 + 0.01164 * pow(T1,2) - 0.001341 * pow(T1,3) + 0.00008723 * pow(T1,4) - 3.017e-6  * pow(T1,5) + 5.153e-8 * pow(T1,6) - 3.42e-10 * pow(T1,7);
                const double rateVA = -5.4 + 1.8*T1 - 0.2124*pow(T1,2) + 0.01015 * pow(T1,3) - 1.515e-4 * pow(T1,4);

                int day = 1;
                if (t % 2== 0) day = 0; // home
                
                epidemic(K, A, N, V, day, rateSE/2, rateEI/2, rateIR/2, biteRate,  rateSEm/2, rateEIm/2, rateMUv/2, rateASm/2, rateMUa/2, rateVA/2, xv);
                travel(K, A, V, N, day);
                t += 1;
                count+=1;
                int IM = 0;
                if (t % 2 == 0 && count == 14){
                    for (int v = 0; v < V; v++)   
                    { 	
                        fprintf(f, "%d %d %d %d %d %d %d %d %d %d %d\n", v, K[v].s, K[v].e, K[v].i, K[v].r, K[v].Amosquito, K[v].mosquito, K[v].Imosquito, K[v].Emosquito, K[v].Smosquito, t/2);
                        IM += K[v].i;
                    }
                    printf("Day %d , Infected %d\n", t/2, IM);
                    count=0;
                }
                

                
                
                if (IM < 100)
                {  
                    sprintf(fileI, "Initial/initial%d.dat", 2013);
                    f1 = fopen(fileI, "r"); // Binary File Home Work
                    int cases = 0;
                    int cell = 0;
                    while (!feof(f1))
                    {
                        fscanf(f1,"%d %d", &cell, &cases);
                        for (int k = 0; k < cases; k++)
                        {
                            int agent = K[cell].home[k];	
                            A[agent].status  = 2;
                            K[cell].i++;
                        }
                    }
                    fclose(f1);
                }
                
                
                if (IM < 100  && t/2 > 364)
                {  
                    sprintf(fileI, "Initial/initial%d.dat", 2013);
                    f1 = fopen(fileI, "r"); // Binary File Home Work
                    int cases = 0;
                    int cell = 0;
                    while (!feof(f1))
                    {
                        fscanf(f1,"%d %d", &cell, &cases);
                        for (int k = 0; k < cases; k++)
                        {
                            int agent = K[cell].home[k];	
                            A[agent].status  = 2;
                            K[cell].i++;
                        }
                    }
                    fclose(f1);
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
    
    
    // Determine the number of iterations to be done using user input
    long total_iterations = 0;
    // parameters: Iyear, Inet
    // command line should be
    // mpirun -np 15 ./sim2013SD 15 *2013* *1*
    // where 15 is total_iterations, *2013* is Iyear and *1* is  Inet

    total_iterations = strtol(argv[1], NULL, 10);

    

    
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
    perform(total_iterations, current_rank, world_size);
    
    
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


