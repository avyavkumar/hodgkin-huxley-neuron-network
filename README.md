# hodgkin-huxley-neuron-network

The Hodgkin–Huxley model, or conductance-based model, is a mathematical model that describes how action potentials in neurons are initiated and propagated. It is a set of nonlinear differential equations that approximates the electrical characteristics of excitable cells such as neurons and cardiac myocytes, and hence it is a continuous time model. 

Alan Lloyd Hodgkin and Andrew Huxley described the model in 1952 to explain the ionic mechanisms underlying the initiation and propagation of action potentials in the squid giant axon. They received the 1963 Nobel Prize in Physiology or Medicine for this work.

This code generates a small world network with N nodes with k average connections per node. It then simulates N stochastic HH neurons connected in such a topology using the 4th order Runge - Kutta method for <t_max> seconds.
It calculates Mean ISI (Inter Spike Interval), Firing Correlation, Avg. Firing Correlation etc.

The parallelisation process is implemented via POSIX standard-compliant threads and MPI (Message Passing Interface) framework for generating parallel processes of different trials, for different rates of coupling strenghts on each core.   
___________________________________________________________________________________________________________________

source for the excerpt on the Hodgkin-Huxley model for action-potential propagation for neurons:

https://en.wikipedia.org/wiki/Hodgkin-Huxley_model

___________________________________________________________________________________________________________________

The Hodgkin-Huxley Model for the Generation of Action Potentials - 

http://www.st-andrews.ac.uk/~wjh/hh_model_intro/

___________________________________________________________________________________________________________________

The ISI distribution of the stochastic Hodgkin-Huxley neuron - 

http://www.ncbi.nlm.nih.gov/pmc/articles/PMC4189387/

___________________________________________________________________________________________________________________

Runge-Kutta method for the temporal discretization for the approximation of solutions of ODEs - 

https://en.wikipedia.org/wiki/Runge-Kutta_methods

___________________________________________________________________________________________________________________
