#ifndef ENCODER_H
#define ENCODER_H

#include "tree.h"
#include "builder.h"

void encode_satellites(binary_tree_t *tree, FILE *input_file, FILE *output_file);
char *code_sat(binary_tree_t *tree, char *name);
#endif