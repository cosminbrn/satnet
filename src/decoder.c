/* Cosmin-George Baroana */
#include "builder.h"
#include "tree.h"
#include "decoder.h"

/**
 * @brief Prints a node
 *
 * This function recieves a node and prints a node
 * as part of a decoded message
 *
 * @param node The node to be printed
 * @param output_file Output file
 */
static void print_node(node_t *node, FILE *output_file)
{
    if (!node) {
        fprintf(stderr, "Invalid node given to the decoder print_node function\n");
        return;
    }

    if (output_file) {
        fprintf(output_file, "%s", node->name);
    } else {
        fprintf(stdout, "%s", node->name);
    }
}

/**
 * @brief Decodes a given codification
 *
 * This function is a helper for decode_messages and find_common_ancestor.
 *
 * This function recieves a satellite network and a code, and breaks the code
 * according to the given network.
 *
 * @param tree Satellite network
 * @param code Codification
 * @param output_file Output file
 * @param mode DECODE_MESSAGE prints every leaf hit along the way,
 *             DECODE_NODE prints only the node the path ends on
 */
void decode_path(binary_tree_t *tree, char *code, FILE *output_file, decode_mode_t mode)
{
    int len = strlen(code);
    if (len == 0) {
        print_node(tree->root, output_file);
        return;
    }

    // Sets inital node to the root
    node_t *curr = tree->root;
    for (int i = 0; i < len; i++) {
        // If the current code digit is 0, we go to the left
        if (code[i] == '0') {
            if (curr->left) {
                curr = curr->left;
            } else {
                // If we reach a point where we need to go left
                // but there isnt a left child anymore, it means
                // we reached one of the desired nodes, and we print it
                // We also decrement i as to stay in place, at the same digit of the code
                // and we also reset the curr pointer to the root
                print_node(curr, output_file);
                fprintf(output_file, " ");
                i--;
                curr = tree->root;
            }
        } else if (code[i] == '1') {
            // If the current code digit is 1, we go to the right
            if (curr->right) {
                curr = curr->right;
            } else {
                // If we reach a point where we need to go right
                // but there isnt a right child anymore, it means
                // we reached one of the desired nodes, and we print it
                // We also decrement i as to stay in place, at the same digit of the code
                // and we also reset the curr pointer to the root
                print_node(curr, output_file);
                fprintf(output_file, " ");
                i--;
                curr = tree->root;
            }
        } else {
            // If we get here, that means the code doesn't contain only 0s and 1s, so it's invalid
            fprintf(stderr, "Invalid code given\n");
            return;
        }
    }

    // The decoder and the common-ancestor lookup share this function but print differently
    if (mode == DECODE_MESSAGE && curr->left == NULL && curr->right == NULL) {
        print_node(curr, output_file);
    } else if (mode == DECODE_NODE) {
        print_node(curr, output_file);
    }
}

/**
 * @brief Recieves binary codes and decodes them, printing the sats
 *
 * Each 0 in a code is a move to the left in the satellite network and each 1 is a
 * move to the right. Every time a leaf is reached, that satellite is printed and the
 * walk restarts from the root, so one code can contain several satellites.
 * @param tree Satellite network as a binary tree
 * @param input_file Pointer to the input file
 * @param output_file Pointer to the output file
 */
void decode_messages(binary_tree_t *tree, FILE *input_file, FILE *output_file)
{
    // Sanity check
    if (!tree || !output_file || !input_file) {
        fprintf(stderr, "Invalid arguments given to decode_messages\n");
        return;
    }

    // Malloc memory for the code
    char *code = (char *)malloc(sizeof(char) * MAX_CODE_LENGTH);
    if (!code) {
        fprintf(stderr, "Error allocating memory for the code\n");
        return;
    }

    // Read the number of codes with sanity check
    int no_codes;
    if (fscanf(input_file, "%d", &no_codes) != 1) {
        fprintf(stderr, "Error reading the number of codes\n");
        return;
    }

    // Number of codes sanity check
    if (no_codes <= 0) {
        fprintf(stderr, "Invalid number of codes given\n");
        return;
    }

    // Read loop
    for (int i = 0; i < no_codes; i++) {
        // Sanity check
        if (fscanf(input_file, "%s", code) != 1) {
            fprintf(stderr, "Error reading code\n");
            return;
        }

        decode_path(tree, code, output_file, DECODE_MESSAGE);
        fprintf(output_file, "\n");
    }

    free(code);
}