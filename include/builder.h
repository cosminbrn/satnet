#ifndef BUILDER_H
#define BUILDER_H

#include "tree.h"

binary_tree_t *build_network(FILE *input_file);
int tree_size(binary_tree_t *tree);
void swap(node_t *node1, node_t *node2);

#endif