#include "MCMC_sampling.h"

void sample_from_distribution(double** J, double* h, int* seed, int* conf, int n_it, int L, int q)
{

	int i, t, rand_pos, rand_alpha;
	double de;
	
	for (i = 0; i < L; ++i)
	{
		conf[i] = seed[i];
	}
	for (t = 0; t < n_it; ++t)	
	{
		rand_pos = rand()%L;
		rand_alpha = ( conf[rand_pos] + (rand()%(q-1)) +1 )%q; // %(q-1) gives integer between 0 and q-2
		de = - h[rand_pos*q+rand_alpha] + h[rand_pos*q+conf[rand_pos]]; // H = -J -h
		for (i = 0; i < L; ++i)
		{	
			if (i<rand_pos)
			{
				// printf("J[%d][%d] + J[%d][%d]\n",i*q+conf[i], rand_pos*q+rand_alpha, i*q+conf[i], rand_pos*q+conf[rand_pos]);
				de = de - J[i*q+conf[i]][rand_pos*q+rand_alpha] + J[i*q+conf[i]][rand_pos*q+conf[rand_pos]];
			}
			else if (i>rand_pos)
			{	
				// printf("J[%d][%d] + J[%d][%d]\n",rand_pos*q+rand_alpha, i*q+conf[i], rand_pos*q+conf[rand_pos], i*q+conf[i]);
				de = de - J[rand_pos*q+rand_alpha][i*q+conf[i]] + J[rand_pos*q+conf[rand_pos]][i*q+conf[i]];
			} 	
		}
		if ( (de<0) || exp(-de) > (double)rand()/(double)RAND_MAX )
		{
			conf[rand_pos] = rand_alpha;
		}
	}
}



void sample_tree_MCMC(double** J, double* h, node* tree, int size_tree, int* seed, int tau, int L, int q)
{
	int i=0, j;
	int it_count =2;

	if(tree==NULL)
	{
		printf("MCMC_sampling.c - sample_tree_MCMC : tree is empty\n");
		exit(1);
	}
	
	printf("Sampling nodes on tree ...\n");
	// Loop on all non-leaves nodes
	while(tree[i].children!=NULL)
	{

		// printf("\n\ni = %d -- parent = %d -- n_children = %d\n", i, tree[i].parent, tree[i].n_children);
		// Case of the root node
		if(tree[i].parent==-1)
		{	
			for (j = 0; j < L; ++j)
			{
				tree[i].conf[j] = seed[j];
			}
		}

		for(j = 0; j<tree[i].n_children; j++)
		{	
			// printf("--> j = %d -- children = %d\n", j, tree[i].children[j]);
			sample_from_distribution(J, h, tree[i].conf, tree[tree[i].children[j]].conf, tau, L, q);
			printf("s=%d/%d\r",it_count++,size_tree);
			// fflush(stdout);
			// usleep(20000);
		}


		i++;
	}
	printf("done                                        \n");

}