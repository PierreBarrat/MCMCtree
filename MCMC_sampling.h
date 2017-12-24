#ifndef MCMC_SAMPLING_INCLUDED
#define MCMC_SAMPLING_INCLUDED 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "tree_handling.h"

/*
Samples from distribution (J,h) for 'n_it' steps, with 'seed' as a starting point and 'conf' as the end point. 
*/
void sample_from_distribution(double** J, double* h, int* seed, int* conf, int n_it, int L, int q);

/*
Samples from distribution (J,h) for 'n_it' steps, with 'seed' as a starting point and 'conf' as the end point, using Gibbs sampler.
*/
void sample_from_distribution_gibbs(double** J, double* h, double* pg, int* seed, int* conf, int n_it, int L, int q);

/*
Samples one configuration for each node in tree. Number of MCMC iterations between two nodes is 'tau'.
size_tree is the number of nodes in the tree
*/
void sample_tree_MCMC(double** J, double* h, node* tree, int size_tree, int* seed, int tau, int L, int q);

#endif