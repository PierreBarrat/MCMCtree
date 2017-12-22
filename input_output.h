#ifndef INPUT_OUTPUT_INCLUDED
#define INPUT_OUTPUT_INCLUDED

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "tree_handling.h"

int read_graph(char* file_name, double** J, double* h, int L, int q);
void write_tree(char* output_file_name, node* tree, int K, int Z, int tau);
void write_conf(char* output_file_name, node* tree, int K, int Z, int L, int q);

#endif