# Mosquito dynamics with human mobility


This repository contains:
  
  1. A C++ code
  2. A python jupyter notebook 

## 1. The Agent Based Model
An Agent Based Model is adopted in order to explicitly simulate the epidemic spread of the disease as governed by the transmission dynamics of the Dengue virus through human-mosquito interactions and promoted by the population movements across the city. The physical environment in which the epidemic and the mobiliy dynamics take places is the grid `G`  described above. We assume a constant human population `N`, and model each individual as an agent who makes two daily trips between its home `H` and work `W` locations. In our model mosquitoes don't travel.

The C++ code implement an agent-based dengue transmission model in which humans and mosquitoes are represented
as agents and humans go through the epidemic states of dengue. To model dengue dynamics, we use a stochastic population model based on the ordinary differential equation (ODE) framework employed by Barmak et al. (https://www.sciencedirect.com/science/article/pii/S0378437115010420). The physical environment in which the epidemic takes place is the grid Grepresented by the matrix M described above. We assume each agent to be susceptible to the virus initially. Upon challenge with infectious mosquito bites, individuals enter the incubation phase with mean duration of given days, later becoming infectious for given days and finally recovering with life-long immunity.

### 1.1 Requirements
You can run the simulations on multiple cores. Be sure to have installed `open-mpi` on your system.

## 2. The Python Notebook
