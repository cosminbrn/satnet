/* Cosmin-George Baroana */
#include "builder.h"
#include "tree.h"
#include "encoder.h"

/**
 * @brief Returns the sateliite's code
 *
 * This function calculates a satellite's code in a given
 * satellite network and returns it
 *
 * @param tree Satellite network
 * @param name Name of the sattelite
 */
char *code_sat(binary_tree_t *tree, char *name)
{
    // Sanity checks
    if (!name || !tree) {
        fprintf(stderr, "Invalid arguments given to code_sat function\n");
        return NULL;
    }

    // Memory for the code
    char *code = (char *)malloc(sizeof(char) * MAX_CODE_LENGTH);
    if (!code) {
        fprintf(stderr, "Error allocating memory for the code\n");
        return NULL;
    }
    code[0] = '\0';

    // Sanity check
    if (!tree->root) {
        fprintf(stderr, "Invalid tree given to the code_sat function\n");
        return NULL;
    }
    node_t *curr = tree->root;
    int index = 0;
    while (curr) {
        if (strcmp(curr->name, name) == 0) {
            code[index] = '\0';
            return code;
        }
        // Names are matched with strstr, so a name like K1 is also found inside K11.
        // When both children contain the name, check the character right after the match:
        // a digit means it was a false positive (a longer name), so keep searching
        if (strstr(curr->left->name, name) != 0 && strstr(curr->right->name, name) != 0) {
            char *left_case = strstr(curr->left->name, name) + strlen(name);
            char *right_case = strstr(curr->right->name, name) + strlen(name);
            while (1) {
                if (!(*left_case >= '0' && *left_case <= '9')) {
                    code[index++] = '0';
                    curr = curr->left;
                    break;
                } else if (!(*right_case >= '0' && *right_case <= '9')) {
                    code[index++] = '1';
                    curr = curr->right;
                    break;
                }
                if (strstr(left_case, name)) {
                    left_case = strstr(left_case, name);
                    if (left_case != NULL && strlen(left_case) > strlen(name)) {
                        left_case += strlen(name);
                    }
                }
                if (strstr(right_case, name)) {
                    right_case = strstr(right_case, name);
                    if (right_case != NULL && strlen(right_case) > strlen(name)) {
                        right_case += strlen(name);
                    }
                }
            }
            continue;
        }
        // If the satellite name is in the left child, we go to the left
        if (strstr(curr->left->name, name) != 0) {
            code[index++] = '0';
            curr = curr->left;
        // If the satellite name is in the right child, we go to the right
        } else if (strstr(curr->right->name, name) != 0) {
            code[index++] = '1';
            curr = curr->right;
        } else {
            fprintf(stderr, "The satellite is NOT in the given network (2)\n");
            return NULL;
        }
    }

    return NULL;
}

/**
 * @brief Transforms satelitte names into code
 *
 * Reads N satellite names and prints their binary paths concatenated, in the order
 * they were given (the inverse of decode_messages).
 *
 * @param tree Satellite network
 * @param input_file Input file
 * @param output_file Output file
 */
void encode_satellites(binary_tree_t *tree, FILE *input_file, FILE *output_file)
{
    // Sanity check
    if (!tree || !output_file || !input_file) {
        fprintf(stderr, "Invalid arguments given to encode_satellites\n");
        return;
    }

    // Malloc memory for the code
    char *code = (char *)malloc(sizeof(char) * MAX_CODE_LENGTH);
    if (!code) {
        fprintf(stderr, "Error allocating memory for the code\n");
        return;
    }
    code[0] = '\0';

    // Read the number of sattelites with sanity check
    int no_sat;
    if (fscanf(input_file, "%d", &no_sat) != 1) {
        fprintf(stderr, "Error reading the number of satellites\n");
        return;
    }

    // Number of sat sanity check
    if (no_sat <= 0) {
        fprintf(stderr, "Invalid number of sat given\n");
        return;
    }

    // Malloc and sanity check
    char *name = (char *)malloc(sizeof(char) * MAX_SAT_NAME_LENGTH);
    if (!name) {
        fprintf(stderr, "Error allocating memory for satellite name\n");
        return;
    }

    // Read and code loop
    for (int i = 0; i < no_sat; i++) {
        if (fscanf(input_file, "%s", name) != 1) {
            fprintf(stderr, "Error reading satellite name\n");
            return;
        }
        // Temp pointer so we can free the allocated memory from the code_sat function
        char *foo = code_sat(tree, name);
        if (!foo) {
            return;
        }
        // We add this satellites codification to the code
        strcat(code, foo);
        free(foo);
    }

    fprintf(output_file, "%s\n", code);
    free(code);
    free(name);
    return;
}
