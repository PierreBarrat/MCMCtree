#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <getopt.h>
#include "MCMC_sampling.h"
#include "input_output.h"
#include "tree_handling.h"


int test_function();
void read_arguments(int argc, char * const argv[], int* L, int* q, int* M, int* K, int* Z, int* tau, int* T, char* identifier, char* input_file);
void usage(char* prog_name);

int main(int argc, char * const argv[])
{	
	int L,q,M,K,i,size_tree,tau, T, Z;
	char input_file[400]="";
	char identifier[200]="";
	char output_file[400]="";
	char output_file_tree[400]="";
	// node* binary_tree=NULL;
	node* Z_tree=NULL;
	double** J = NULL;
	double* h = NULL;
	int* seed;

	// Initializations
	srand(time(NULL));
	L = 0;
	q = 0;
	M = 0;
	Z = 2;
	K = 0;
	tau = 0;
	T = 100000;

	// Reading arguments
	read_arguments(argc, argv, &L, &q, &M, &K, &Z, &tau, &T, identifier, input_file);
	printf("MCMC_tree.c - main : L=%d, q=%d, M=%d, K=%d, Z=%d, tau=%d, T=%d, id=%s, input_file=%s\n", L, q, M, K, Z, tau, T, identifier, input_file);
	// Creating tree
	// binary_tree = create_tree_binary(K, L*q);
	Z_tree = create_tree_z(K,Z,L);
	size_tree = (pow(Z,K)-1)/(Z-1);
	
	// Creating graph
	h = malloc(L*q*sizeof(*h));
	J = malloc(L*q*sizeof(*J));
	if(h==NULL || J==NULL)
		{printf("MCMC_tree.c - main : Error in allocating graph 1\n"); return 1;}
	for (i = 0; i < L*q; ++i)
	{
		J[i] = malloc(L*q*sizeof(*J[0]));
		if(J[i]==NULL)
			{printf("MCMC_tree.c - main : Error in allocating graph 1\n"); return 1;}
	}
	// printf("J is %dx%d -- h is 1x%d\n", L*q, L*q, L*q);

	// Reading graph file
	if (read_graph(input_file, J, h, L, q)!=0)
		{printf("MCMC_tree.c - main : Error in read_graph\n"); return 1;}


	// Defining seed
	seed = malloc(L*sizeof(*seed));
	for (i = 0; i < L; ++i)
	{
		seed[i]=0;
	}
	sample_from_distribution(J, h, seed, seed, T, L, q);

	// Sampling for each node of tree
	sample_tree_MCMC(J, h, Z_tree, size_tree, seed, tau, L, q);

	// Writing result to output file
	sprintf(output_file, "MCMC_tree_sample_n%d_q%d_K%d_Z%d_tau%d_T%d_%s.txt",L,q,K,Z,tau,T,identifier);
	sprintf(output_file_tree, "MCMC_tree_struct_n%d_q%d_K%d_Z%d_tau%d_T%d_%s.txt",L,q,K,Z,tau,T,identifier);
	write_conf(output_file, Z_tree, K, Z, L, q);
	write_tree(output_file_tree, Z_tree, K, Z, tau);
	// Freeing stuff
	for (i = 0; i < L*q; ++i)
	{
		free(J[i]);
	}
	free(J);
	free(h);
	free_tree(Z_tree, size_tree);


	// test_function();

	return 0;
}

void read_arguments(int argc,char * const argv[], int* L, int* q, int* M, int* K, int* Z, int* tau, int* T, char* identifier, char* input_file)
{
	int c;
	while ((c = getopt(argc, argv, "n:q:m:K:Z:t:T:f:hu:")) != -1) {
		switch (c) {
			case 'n':
				*L = atoi(optarg);
				break;
			case 'q':
				*q = atoi(optarg);
				break;
			case 'm':
				*M = atoi(optarg);
				break;
			case'K':
				*K = atoi(optarg);
				break;
			case 'Z':
				*Z = atoi(optarg);
				break;
			case 'T':
				*T = atoi(optarg);
				break;
			case 't':
				*tau = atoi(optarg);
				break;
			case 'f':
				strcpy(input_file, optarg);
				break;
			case 'u':
				strcpy(identifier, optarg);
				break;
			case '?':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
			default:
				usage(argv[0]);
				exit(EXIT_FAILURE);
		}
	}
}

void usage(char* prog_name)
{
	printf("Usage: %s [options]\n",prog_name);
	printf("MCMC sampling of an Ising/Potts model on a tree\n\n");
	printf("arguments: (* : required) \n");
	printf("-n <n>* : number of nodes of the Ising/Potts model.\n");
	printf("-q <q>* : number of states for each spin.\n");
	printf("-K <K>* : Number of layers of tree.\n");
	printf("-Z <Z>* : Degree of nodes.\n");
	printf("-t <tau>* : Number of MCMC steps between each tree node (ie branch length).\n");
	printf("-f <str>* : Name of input file.\n");
	printf("-T <T> : Number of initial iterations. Default 100000.\n");
	printf("-u <string> : Identifier for output file.\n");
	printf("-?/-h : This help.\n\n");
}



// int test_function()
// {
// 	int test[5] = {1,1,2,3,0};
// 	FILE* file;
// 	file = fopen("test.txt","w");
// 	fwrite(test, sizeof(int), 5, file);
// 	// printf("test : %s\n", test);


// 	return 0;
// }