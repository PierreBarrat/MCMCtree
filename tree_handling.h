#ifndef TREE_HANDLING_INCLUDED
#define TREE_HANDLING_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
children of leaves are set to NULL in tree creation
parent of the root is -1
*/
typedef struct node
{
	int parent;
	int* children;
	int n_children;
	int* conf;
}node;

/*
K is the number of layers of the tree. 
Number of leaves is 2^(K-1)
*/
node* create_tree_binary(int K, int size_conf); 

/*
K is the number of layers of the tree. 
Z the degree of nodes (ie number of children).
Number of leaves is Z^(K-1)
Number of nodes is (Z^K-1)/(Z-1).
*/
node* create_tree_z(int K, int Z, int size_conf);

/*
Every created tree should be freed using this 
*/
void free_tree(node* tree, int size_tree);



#endif

