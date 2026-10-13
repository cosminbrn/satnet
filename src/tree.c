/* Cosmin-George Baroana */
#include "tree.h"
#include "builder.h"

/**
 * @brief Allocates memory for a node
 *
 * This function allocates memory and checks if the memory is correctly allocated
 * for a node_t variable
 * @return A pointer to the freshly allocated memory space
 */
node_t *create_node(void)
{
    // Allocated memory, and sanity check
    node_t *new = (node_t *)malloc(sizeof(node_t) * 1);
    if (!new) {
        fprintf(stderr, "Error allocating memory for a node_t\n");
        return NULL;
    }

    // Init values
    new->priority = 0;
    new->name = (char *)malloc(sizeof(char) * MAX_NAME_LENGTH);
    if (!new->name) {
        fprintf(stderr, "Error allocating memory for the name field of a node\n");
        return NULL;
    }
    new->left = NULL;
    new->right = NULL;
    return new;
}

/**
 * @brief Frees memory of a node
 *
 * This function reserves a node_t pointer and frees the memory at that address
 * @param node The node to be freed
 */
void free_node(node_t *node)
{
    // Argument validation
    if (!node) {
        fprintf(stderr, "Argument given is equal to NULL\n");
        return;
    }

    // Sanity check
    if (!node->name) {
        fprintf(stderr, "Arguemtn given to free_node is invalid, no name allocated\n");
        return;
    }
    // Frees the node
    free(node->name);
    node->name = NULL;
    node->left = NULL;
    node->right = NULL;
    free(node);
}

/**
 * @brief Allocates memory for a binary tree
 *
 * This function allocated memory and checks if memory was succesfully allocated
 * for a binary_tree_t variable
 * @return A pointer to the freshly allocated memory
 */
binary_tree_t *create_binary_tree(void)
{
    // Allocated memory and sanity check
    binary_tree_t *tree = (binary_tree_t *)malloc(sizeof(binary_tree_t) * 1);
    if (!tree) {
        fprintf(stderr, "Error allocating memory for a binary_tree_t\n");
        return NULL;
    }

    // Init values
    tree->size = 0;
    tree->root = NULL;

    return tree;
}

/**
 * @brief Frees memory of a binary tree
 *
 * Helper function for the function with the same name from above
 *
 * @param node Node to free after all the subnodes are freed
 */
static void __free_binary_tree(node_t *node)
{
    // End condition
    if (!node) {
        return;
    }

    // Frees tree from the bottom, left to right
    __free_binary_tree(node->left);
    __free_binary_tree(node->right);
    free_node(node);
}

/**
 * @brief Frees memory of a binary tree
 *
 * This function goes through all the nodes of a binary tree and frees
 * all the nodes inside it, as well as the actual pointer that points to the
 * binary_tree_t variable
 * @param tree The pointer to the binary_tree_t structure
 */
void free_binary_tree(binary_tree_t *tree)
{
    // Argument validation
    if (!tree) {
        fprintf(stderr, "Argument given to free_binary_tree is NULL already\n");
        return;
    }

    // Sanity check
    if (!tree->root) {
        fprintf(stderr, "Tree is already empty\n");
        return;
    }

    // Call helper function
    __free_binary_tree(tree->root);

    tree->root = NULL;
    free(tree);
    tree = NULL;
}

/**
 * @brief Prints a node
 *
 * This function recieves a node and prints it's values
 * in the "priority-name" format
 *
 * @param Node Node to be printed
 */

static void print_node(node_t *node, FILE *output_file)
{
    // Argument validation
    if (!node) {
        fprintf (stderr, "Invalid node given to the print function\n");
        return;
    }

    // This function was used for debugging, so it can also print to stdout
    if (output_file == NULL) {
        fprintf(stdout, "%d -- %s\n", node->priority, node->name);
    } else {
        fprintf(output_file, "%d-%s", node->priority, node->name);
    }
    return;
}

/**
 * @brief Prints the nodes of a binary tree in Breadth-First Traversal (BFT) order.
 *
 * This function traverses the given binary tree level by level (BFT)
 * and prints each node's value and priority to the specified output file.
 * Each level of the tree is printed on a new line, and nodes within the same
 * level are separated by spaces.
 *
 * @param tree Pointer to the binary_tree_t structure representing the tree.
 * @param output_file Pointer to the FILE where the output will be printed.
 */
void print_tree_BFT(binary_tree_t *tree, FILE *output_file)
{
    // Argument validation
    if (!tree || !tree->root) {
        fprintf(stderr, "Invalid argument given in print_tree_BFT\n");
        return;
    }
    // Gets tree size
    tree->size = tree_size(tree);
    // Allocate queue as an array of pointers only, not new nodes
    node_t **queue = (node_t **)malloc(sizeof(node_t *) * tree->size);
    if (!queue) {
        fprintf(stderr, "Error allocating memory for queue in print_tree_BFT\n");
        return;
    }
    // The function uses a dynamically allocated queue to store pointers to the nodes
    // as it traverses the tree.

    // Queue size
    int size_index = 0;

    // What node to print is decided by this index
    int print_index = 0;

    // These two help with the \n and " " for the output
    int nodes_current_level = 1;
    int nodes_next_level = 0;

    // Init the queue with the root
    queue[size_index] = tree->root;
    while (print_index != tree->size) {
        // Print the node at the start of the queue
        print_node(queue[print_index], output_file);

        // Add its children to the queue, if they exist, similar to a PUSH instruction
        // We also count how many nodes the next leve will have
        if (queue[print_index]->left != NULL) {
            queue[++size_index] = queue[print_index]->left;
            nodes_next_level++;
        }
        if (queue[print_index]->right != NULL) {
            queue[++size_index] = queue[print_index]->right;
            nodes_next_level++;
        }

        // If we reached the end of the current level, we add a newline and reset the counters
        if (--nodes_current_level == 0) {
            fprintf(output_file, "\n");
            nodes_current_level = nodes_next_level;
            nodes_next_level = 0;
        } else {
            // If we DID NOT reach the end of the current level, we print a space
            fprintf(output_file, " ");
        }
        // Advance in the queue, similar to a POP instruction
        print_index++;
    }
    free(queue);
}

/**
 * @brief Allocate memory for an array of nodes
 *
 * This function receives a size and allocated memory for
 * size nodes in an array
 * @param size How many nodes need memory allocated
 * @return The array of pointers that points to nodes
 */
node_t **create_node_array(int size)
{
    // Allocate memory and sanity check
    node_t **arr = (node_t **)malloc(sizeof(node_t *) * size);
    if (!arr) {
        fprintf(stderr, "Error allocating memory for the node_array\n");
        return NULL;
    }

    // Allocate memory and sanity check, in which we free the previous mallocs if
    // even one of them fails
    for (int i = 0; i < size; i++) {
        arr[i] = create_node();
        if (!arr[i]) {
            fprintf(stderr, "Error allocating node array elements\n");
            for (int j = i - 1; j >= 0; j--) {
                free_node(arr[j]);
            }
            free(arr);
            return NULL;
        }
    }

    return arr;
}

/**
 * @brief Frees node_t array
 *
 * This function recieves a node_t array and frees the memory of each element
 * in it, including the given pointer
 * @param arr Pointer to arr that needs to be freed
 * @param size Size of the array
 */
void free_node_array(node_t **arr, int size)
{
    // Allocate memory and sanity check
    if (!arr) {
        fprintf(stderr, "Invalid parameter given to free_node_array function\n");
        return;
    }

    // Complex free loop, because some nodes can point to the same memory. We avoid
    // this unpleasant case here
    for (int i = 0; i < size; i++) {
        if (arr[i]) {
            free_node(arr[i]);
            for (int j = i + 1; j < size; j++) {
                if (arr[i] == arr[j]) {
                    arr[j] = NULL;
                }
            }
            arr[i] = NULL;
        }
    }
    free(arr);
    return;
}

/**
 * @brief Prints an array of node_t pointers.
 *
 * This function iterates through an array of pointers to node_t structures
 * and prints each node using the print_node function. If the array pointer
 * is NULL or the size is less than or equal to zero, an error message is
 * printed to stderr.
 *
 * @param arr  Pointer to an array of node_t pointers.
 * @param size Number of elements in the array.
 */
void print_node_array(node_t **arr, int size)
{
    // Argument validation
    if (!arr || size <= 0) {
        fprintf(stderr, "Invalid arguments given to print_node_array function\n");
        return;
    }

    for (int i = 0; i < size; i++) {
        print_node(arr[i], NULL);
    }
    return;
}