/* Cosmin-George Baroana */
#include "builder.h"
#include "tree.h"
#include "decoder.h"
#include "encoder.h"
#include "ancestor.h"

#define USAGE "Usage: ./satnet [-c1 | -c2 | -c3 | -c4] [input_file] [output_file]\n"

/**
 * @brief Reads command-line arguments and parses them
 *
 * The main function reads the command-line arguments, builds the satellite network
 * from the input file and then runs the selected operation on it
 * The parameters are the usual ones
 */
int main(int argc, char *argv[])
{
    // Check if the correct number of arguments is provided
    if (argc != 4) {
        fprintf(stderr, USAGE);
        return 1;
    }

    char *option = argv[1];

    // Validate the option (must be one of -c1 ... -c4)
    if (strcmp(option, "-c1") != 0 && strcmp(option, "-c2") != 0 &&
        strcmp(option, "-c3") != 0 && strcmp(option, "-c4") != 0) {
        fprintf(stderr, "Invalid option. Use -c1, -c2, -c3 or -c4.\n");
        fprintf(stderr, USAGE);
        return 1;
    }

    // Open the input file
    FILE *input_file = fopen(argv[2],  "r");
    if (!input_file) {
        fprintf(stderr, "Error opening input file\n");
        return 1;
    }

    // Open the output file
    FILE *output_file = fopen(argv[3], "wt");
    if (!output_file) {
        fprintf(stderr, "Error opening output file\n");
        fclose(input_file);
        return 1;
    }

    // Every operation works on the network, so it is always built first
    binary_tree_t *tree = build_network(input_file);
    if (!tree) {
        fprintf(stderr, "Error building the satellite network\n");
        fclose(input_file);
        fclose(output_file);
        return 1;
    }

    // Handle the different options based on the command-line argument
    if (strcmp(option, "-c1") == 0) {
        // Print the tree in Breadth-First Traversal order
        print_tree_BFT(tree, output_file);
    } else if (strcmp(option, "-c2") == 0) {
        // Decode binary paths into satellite names
        decode_messages(tree, input_file, output_file);
    } else if (strcmp(option, "-c3") == 0) {
        // Encode satellite names into binary paths
        encode_satellites(tree, input_file, output_file);
    } else if (strcmp(option, "-c4") == 0) {
        // Find the closest common ancestor of a group of satellites
        find_common_ancestor(tree, input_file, output_file);
    }

    // Free the binary tree and close the files
    free_binary_tree(tree);
    fclose(input_file);
    fclose(output_file);

    return 0;
}
