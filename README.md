# Mosquito dynamics with human mobility


This repository contains:
  
  1. A C++ code
  2. A python jupyter notebook 

## 1. The Agent Based Model
The C++ code implement an agent-based dengue transmission model in which humans and mosquitoes are represented
as agents and humans go through the epidemic states of dengue. To model dengue dynamics, we use a stochastic population model based on the ordinary differential equation (ODE) framework employed by Barmak et al. (https://www.sciencedirect.com/science/article/pii/S0378437115010420). . The physical environment in which the epidemic takes place is the grid $G$ represented by the matrix $M$ described above. We assume each agent to be susceptible to the virus initially ($S^h$). Upon challenge with infectious mosquito bites ($\lambda^{v \rightarrow h}$), individuals enter the incubation phase ($E^h$) with mean duration of days $1/\gamma^h$, later becoming infectious ($I^h$) for days $1/\sigma^h$ and finally recovering ($R^h$) with life-long immunity.

1. Generate Inputs
