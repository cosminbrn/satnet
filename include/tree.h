#ifndef TREE_H
#define TREE_H
#define MAX_NAME_LENGTH 1000
#define MAX_CODE_LENGTH 1000
#define MAX_SAT_NAME_LENGTH 15

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node_t {
    int priority;
    char *name;
    struct node_t *left;
    struct node_t *right;
} node_t;

typedef struct binary_tree_t {
    node_t *root;
    int size;
} binary_tree_t;

node_t *create_node(void);
void free_node(node_t *node);
binary_tree_t *create_binary_tree(void);
void free_binary_tree(binary_tree_t *tree);
node_t **create_node_array(int size);
void free_node_array(node_t **arr, int size);
void print_tree_BFT(binary_tree_t *tree, FILE *output_file);
void print_node_array(node_t **arr, int size);

#endif