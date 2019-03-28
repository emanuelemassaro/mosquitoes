
1. Install OPEN MPI 
brew install open-mpi


2. Compile the code
mpicxx -o mainreal main1.cpp Node.cpp Agent.cpp -std=gnu++11

3. Run the code
mpirun -np 1 ./mainreal 1


-np 1 (Number of processors)
1 (Number of runs)
