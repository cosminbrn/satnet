#ifndef DECODER_H
#define DECODER_H

#include "tree.h"
#include "builder.h"

typedef enum {
    DECODE_MESSAGE, // print every satellite reached while walking the code
    DECODE_NODE     // print only the node the code path ends on
} decode_mode_t;

void decode_messages(binary_tree_t *tree, FILE *input_file, FILE *output_file);
void decode_path(binary_tree_t *tree, char *code, FILE *output_file, decode_mode_t mode);

#endif