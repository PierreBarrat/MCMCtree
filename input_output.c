#include "input_output.h"

int read_graph(char* filename, double** J, double* h, int L, int q)
{
	FILE* file_handle=NULL;
	int i,j,alpha,beta, check_count = 0;
	double temp_J;
	char temp_char;

	printf("Reading graph ...");
	// File handling
	file_handle = fopen(filename, "r");
	if(file_handle==NULL)
	{
		printf("Error in input_output.c read_graph: file failed to open\n");
		return 1;
	}

	// while((temp_char = fgetc(file_handle))!=EOF)
	// {	
	// 	printf("%c",temp_char);
	// }
	// return 1;
	// Retrieving couplings
	temp_char = fgetc(file_handle);
	while(temp_char=='J')
	{	
		fscanf(file_handle," %d %d %d %d %lf \n",&i,&j,&alpha,&beta,&temp_J);
		J[i*q+alpha][j*q+beta] = temp_J;
		J[j*q+beta][i*q+alpha] = temp_J;
		check_count++;
		temp_char = fgetc(file_handle);
	}		
	if(check_count!=L*(L-1)*q*q/2)
	{
		printf("input_output.c - read_graph: incorrect number of couplings -- check_count = %d\n",check_count);
		return 1;
	}
	for (i = 0; i < L; ++i)
	{
		for (alpha = 0; alpha < q; ++alpha)
		{
			for (beta = 0; beta < q; ++beta)
			{
				J[i*q+alpha][i*q+beta] = 0;
			}
		}
	}

	// Retrieving fields
	while(temp_char=='h')
	{
		fscanf(file_handle," %d %d %lf\n",&i,&alpha,&temp_J);
		h[i*q+alpha] = temp_J;
		check_count++;
		temp_char = fgetc(file_handle);
	}
	if(check_count!=L*(L-1)*q*q/2+L*q)
	{
		printf("input_output.c - read_graph: incorrect number of fields -- check_count = %d\n",check_count);
		return 1;
	}

	fclose(file_handle);

	printf("Done\n");

	return 0;

	/*
	DEBUG - Prints graph
	*/
	// for(i=0;i<L;++i)
	// {
	// 	for (alpha = 0; alpha < q; ++alpha)
	// 	{
	// 		printf("h[%d][%d] = %lf\n",i,alpha, h[i*q+alpha]);
	// 		for (j = 0; j < L; ++j)		
	// 		{
	// 			for (beta = 0; beta < q; ++beta)
	// 			{
	// 				printf("J[%d][%d][%d][%d] = %lf\n",i,j,alpha,beta,J[i*q+alpha][j*q+beta]);
	// 			}
	// 		}
	// 	}
	// }

}



void write_tree(char* output_file_name, node* tree, int K, int Z, int tau)
{
	int n_nodes, n_leaves, i, count;
	FILE* output_handle;

	output_handle = fopen(output_file_name,"w");
	if(output_handle==NULL)
		{printf("inpout_output.c - write_tree : could not open output file %s\n", output_file_name); exit(EXIT_FAILURE);}
	fprintf(output_handle,"# Node Conf. Parent Branch_length\n");

	n_nodes = (int)((pow(Z,K)-1)/(Z-1));
	n_leaves = (int)pow(Z,K-1);
	count = 1;
	for (i = 0; i < n_nodes; ++i)
	{
		if (tree[i].children==NULL)
		{
			fprintf(output_handle, "%d %d %d %d\n", i, count, tree[i].parent, tau);
			count+=1;
		}
		else
			fprintf(output_handle, "%d -1 %d %d\n", i, tree[i].parent, tau);
	}
}

void write_conf(char* output_file_name, node* tree, int K, int Z, int L, int q)
{
	int i,s, size_tree, a, id_tree,n_leaves;
	char output_file_name_wholetree[512];
	FILE* output_file_handle; 
	FILE* output_file_handle_wholetree;

	size_tree = (pow(Z,K)-1)/(Z-1);
	printf("size_tree = %d\n",size_tree);
	strcpy(output_file_name_wholetree, output_file_name);
	strcat(output_file_name_wholetree, "_WHOLETREE");
	// printf("Z = %d -- K = %d -- size_tree = %d\n",Z,K,size_tree);

	printf("size_tree = %d - - %d\n",size_tree,K);
	// printf("tree[O].children = %d\n",tree[0].children[0]);

	output_file_handle = fopen(output_file_name, "w");
	output_file_handle_wholetree = fopen(output_file_name_wholetree,"w");
	if(output_file_handle==NULL)
		{printf("inpout_output.c - write_conf : could not open output file %s\n", output_file_name); exit(EXIT_FAILURE);}
	if(output_file_handle_wholetree==NULL)
		{printf("inpout_output.c - write_conf : could not open output file %s\n", output_file_name_wholetree); exit(EXIT_FAILURE);}	

	n_leaves = (int)(int)pow(Z,K-1);
	fprintf(output_file_handle,"# %d %d %d\n", n_leaves, L, q);
	// Writing leaves configurations
	for (i = 0; i < size_tree; ++i)
	{	
		// printf("i= %d -- tree[%d].conf[1] = %d\n",i,i,tree[i].conf[1]);
		if(tree[i].children==NULL)
		{
			for (s = 0; s < L; ++s)
			{
				fprintf(output_file_handle, "%d", tree[i].conf[s]);
				if(s!=L-1)
				{fprintf(output_file_handle, " ");}
			}
			fprintf(output_file_handle,"\n");
		}
	}
	
	// Writing all configurations, with numbering
	for(a = 1; a < K+1; ++a)
	{
		for(i = 0; i < pow(Z,a-1); ++i)
		{	
			fprintf(output_file_handle_wholetree, "%d %d ", a, i);
			id_tree = i + (int)(pow(Z,a-1)-1)/(Z-1);
			// printf("a = %d -- i = %d -- id_tree = %d\n", a, i, id_tree);
			for (s = 0; s < L; ++s)
			{	
				// printf("tree[%d].conf[%d] = %d\n",id_tree,s,tree[id_tree].conf[s]);
				fprintf(output_file_handle_wholetree, "%d", tree[id_tree].conf[s]);
				if(s!=L-1)
				{fprintf(output_file_handle_wholetree, " ");}
			}
			fprintf(output_file_handle_wholetree,"\n");

		}
	}

	fclose(output_file_handle);
	fclose(output_file_handle_wholetree);
}