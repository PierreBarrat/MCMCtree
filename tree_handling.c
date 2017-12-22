#include "tree_handling.h"

node* create_tree_binary(int K, int size_conf)
{	
	int count_parent = 0, count_children = 0, i, k;
	int n_nodes, n_leaves;
	node* tree=NULL;

	n_nodes = (int)pow((double)2,K)-1;
	n_leaves = (int)pow((double)2,K-1);


	tree = malloc(n_nodes*sizeof(node));
	if(tree==NULL)
	{
		printf("tree_handling.c : malloc failed\n");
		return tree;
	}

	// Initialization : allocating memory for children, setting children of leaves to NULL, etc ... 
	for (i = 0; i < n_nodes-n_leaves; ++i)
	{
		tree[i].children = malloc( 2*sizeof(*((node*)0)->children) ); // sizeof(*children) basically. Allows changing type of children.  
		if(tree[i].children==NULL)
			{printf("tree_handling.c : children malloc failed\n"); return(NULL);}
		tree[i].conf = malloc( size_conf*sizeof(*((node*)0)->conf) );
		if(tree[i].conf==NULL)
			{printf("tree_handling.c : conf malloc failed\n"); return(NULL);}


		tree[i].parent = -1;
		tree[i].children[1] = 0;
		tree[i].children[1] = 0;
		tree[i].n_children = 2;
		for (k = 0; k < size_conf; ++k)
		{
			tree[i].conf[k] = 0;
		}
	}
	for (i = n_nodes-n_leaves; i < n_nodes; ++i)
	{
		tree[i].parent = -1;
		tree[i].children = NULL;
		tree[i].n_children = 0;
		tree[i].conf = malloc( size_conf*sizeof(*((node*)0)->conf) );
		if(tree[i].conf==NULL)
			{printf("tree_handling.c : conf malloc failed\n"); return(NULL);}
	}

	/*
	 Defining parents and children.
	 i is the number of the parent layer considered. Goes from 1 up to K-1.
	 Number of nodes below ith layer : pow(2,i-1)-1. 
	 Number of nodes inside ith layer : pow(2,i-1). 
	 */
	for (i = 1; i < K; ++i)
	{	
		for (count_parent = pow(2,i-1)-1; count_parent < pow(2,i-1)-1+pow(2,i-1); ++count_parent)
		{	
			count_children = pow(2,i)-1 + 2*(count_parent-pow(2,i-1)+1);
			tree[count_parent].children[0] = count_children;
			tree[count_parent].children[1] = count_children+ 1;
			tree[count_children].parent = count_parent;
			tree[count_children+1].parent = count_parent;	
		}
	}

	return tree;
	
	// // // TEST CODE -- Prints tree to output
	// for (i = 0; i < n_nodes-n_leaves; ++i)
	// {
	// 	printf("tree(%d).parent = %d -- tree(%d).children(1) = %d -- tree(%d).children(2) = %d\n", i, tree[i].parent, i, tree[i].children[0], i, tree[i].children[1]);
	// }
	// for (i = n_nodes-n_leaves; i < n_nodes; ++i)
	// {
	// 	printf("tree(%d).parent = %d\n", i, tree[i].parent);
	// }
}

node* create_tree_z(int K, int Z, int size_conf)
{	
	int count_parent = 0, count_children = 0, i, k, z;
	int n_nodes, n_leaves, n_above_i, n_in_i;
	double test2;
	node* tree=NULL;

	test2 = pow(Z,K)-1;
	n_nodes = (int) (test2/(Z-1));
	// n_nodes = (int)((double)pow(Z,K)-1/(Z-1));
	n_leaves = (int)pow(Z,K-1);
	// printf("test2 = %d -- n_nodes = %d -- n_leaves = %d\n",(int) test2,n_nodes, n_leaves);
	tree = malloc(n_nodes*sizeof(node));
	if(tree==NULL)
	{
		printf("tree_handling.c : malloc failed\n");
		return tree;
	}
	// Initialization : allocating memory for children, setting children of leaves to NULL, etc ... 
	for (i = 0; i < n_nodes-n_leaves; ++i)
	{
		tree[i].children = malloc( Z*sizeof(*(tree[i].children)) ); // sizeof(*children) basically. Allows changing type of children.  
		if(tree[i].children==NULL)
			{printf("tree_handling.c : children malloc failed\n"); return(NULL);}
		tree[i].conf = malloc( size_conf*sizeof(*(tree[i].conf)) );
		if(tree[i].conf==NULL)
			{printf("tree_handling.c : conf malloc failed\n"); return(NULL);}

		tree[i].parent = -1;
		for(z = 0; z < Z; ++z)
		{
			tree[i].children[z] = 0;
		}
		tree[i].n_children = Z;
		for (k = 0; k < size_conf; ++k)
		{
			tree[i].conf[k] = 0;
		}
	}
	for (i = n_nodes-n_leaves; i < n_nodes; ++i)
	{
		tree[i].parent = -1;
		tree[i].children = NULL;
		tree[i].n_children = 0;
		tree[i].conf = malloc( size_conf*sizeof(*((node*)0)->conf) );
		if(tree[i].conf==NULL)
			{printf("tree_handling.c : conf malloc failed\n"); return(NULL);}
	}
	printf("n_nodes = %d\n -- i=%d\n",n_nodes,i);
	/*
	 Defining parents and children.
	 i is the number of the parent layer considered. Goes from 1 up to K-1.
	 Number of nodes above ith layer : (pow(Z,i-1)-1)/(Z-1). 
	 Number of nodes inside ith layer : pow(Z,i-1). 
	 */
	for (i = 1; i < K; ++i)
	{	
		n_above_i = (pow(Z,i-1)-1)/(Z-1);
		n_in_i = pow(Z,i-1);
		for (count_parent = n_above_i; count_parent < n_above_i+n_in_i; ++count_parent) // All nodes inside ith layer
		{	
			count_children = (pow(Z,i)-1)/(Z-1) + Z*(count_parent-n_above_i);
			for(z = 0; z < Z; ++z)
			{
				tree[count_parent].children[z] = count_children + z;
				tree[count_children+z].parent = count_parent;
			}	
		}
	}

	
	// TEST CODE -- Prints tree to output
	// printf("\n\n");
	// for (i = 0; i < n_nodes-n_leaves; ++i)
	// {
	// 	printf("tree(%d).parent = %d -- tree(%d).children(1) = %d -- tree(%d).children(2) = %d -- tree(%d).children(3) = %d\n", i, tree[i].parent, i, tree[i].children[0], i, tree[i].children[1], i, tree[i].children[2]);
	// }
	// for (i = n_nodes-n_leaves; i < n_nodes; ++i)
	// {
	// 	printf("tree(%d).parent = %d\n", i, tree[i].parent);
	// }
	// printf("\n\n");

	return tree;
}

void free_tree(node* tree, int size_tree)
{
	int i;
	for (i = 0; i < size_tree; ++i)
	{	
		if(tree[i].children!=NULL)
		{
			free(tree[i].children);
		}
		free(tree[i].conf);
	}
	free(tree);
}