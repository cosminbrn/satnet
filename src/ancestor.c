/* Cosmin-George Baroana */
#include "builder.h"
#include "tree.h"
#include "encoder.h"
#include "ancestor.h"
#include "decoder.h"

/**
 * @brief Finds longest_common_substring_from_index_zero
 *
 * This function takes 2 strings and finds their longest common prefix
 *
 * @param string1 String 1
 * @param string2 String 2, the string that will contain the end result
 * @return String2, in which we add a string terminator to cut it off where we want it to be cut off
 */
char *longest_common_substring_from_index_zero(char *string1, char *string2)
{
    int index = 0;
    int min_length = strlen(string1) < strlen(string2) ? strlen(string1) : strlen(string2);
    for (index = 0; index < min_length; index++) {
        if (string1[index] != string2[index]) {
            // Stops the function at the first different index
            break;
        }
    }
    // Sets that index to the string terminator, so we now have the longest_common_substring_from_index_zero
    string2[index] = '\0';
    return string2;
}

/**
 * @brief Finds common parent node
 *
 * Reads a list of satellites and prints the closest node that connects all of them
 * (their lowest common ancestor). Each satellite is encoded as its root-to-leaf path,
 * and the ancestor is the node at the end of the longest common prefix of those paths.
 *
 * @param tree Satellite network
 * @param input_file Input file
 * @param output_file Output file
 */
void find_common_ancestor(binary_tree_t *tree, FILE *input_file, FILE *output_file)
{
    // Sanity check
    if (!tree || !output_file || !input_file) {
        fprintf(stderr, "Invalid arguments given to find_common_ancestor\n");
        return;
    }

    // Read the number of sattelites with sanity check
    int no_sat;
    if (fscanf(input_file, "%d", &no_sat) != 1) {
        fprintf(stderr, "Error reading the number of satellites\n");
        return;
    }

    // Number of sat sanity check
    if (no_sat <= 1) {
        fprintf(stderr, "Invalid number of sat given\n");
        return;
    }

    // Malloc memory for the code
    char *code = (char *)malloc(sizeof(char) * MAX_CODE_LENGTH);
    if (!code) {
        fprintf(stderr, "Error allocating memory for the code\n");
        return;
    }
    code[0] = '\0';

    char *name = (char *)malloc(sizeof(char) * MAX_SAT_NAME_LENGTH);
    if (!name) {
        fprintf(stderr, "Error allocating memory for satellite name\n");
        return;
    }

    // Read the first sat name
    if (fscanf(input_file, "%s", name) != 1) {
        fprintf(stderr, "Error reading satellite name\n");
    }

    // Code it
    char *foo = code_sat(tree, name);
    // Copy the code
    strcpy(code, foo);
    free(foo);

    for (int i = 1; i < no_sat; i++) {
        if (fscanf(input_file, "%s", name) != 1) {
            fprintf(stderr, "Error reading satellite name\n");
        }
        // Temporary pointer so we can free the memory which the code_sat function allocs
        char *foo = code_sat(tree, name);
        // We take each sat, code it, and find the common parent by being the longest common substring
        // starting from [0]
        strcpy(code, longest_common_substring_from_index_zero(code, foo));
        free(foo);
    }
    decode_path(tree, code, output_file, DECODE_NODE);
    free(name);
    free(code);
}
