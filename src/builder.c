/* Cosmin-George Baroana */
#include "builder.h"
#include "tree.h"

/**
 * @brief This function swaps the values in two nodes
 *
 * @param node1 The values of this node will be moved to node 2
 * @param node2 The values of this node will be moved to node 1
 */
void swap(node_t *node1, node_t *node2)
{
    int auxp;
    auxp = node1->priority;
    node1->priority = node2->priority;
    node2->priority = auxp;
    char *auxs;
    auxs = node1->name;
    node1->name = node2->name;
    node2->name = auxs;
    node_t *auxn;
    auxn = node1->left;
    node1->left = node2->left;
    node2->left = auxn;
    auxn = node1->right;
    node1->right = node2->right;
    node2->right = auxn;
}

/**
 * @brief Applies heap quirks on a given array
 *
 * This function recevies a heap in node_t array form and
 * applies the properties of the min-heap on a given position
 * @param heap Array of nodes
 * @param pos Node on which we check the properties
 */
void min_heapify(node_t **heap, int pos)
{
    // The nodes need to respect the conditions of a minheap, so we get the positions and
    // we check the said conditions
    int smallest = pos;
    int left = pos * 2 + 1;
    int right = pos * 2 + 2;

    // If the left child doesn't respect the minheap condition compared to its parent
    // we pseudo-swap them
    if (heap[smallest]->priority > heap[left]->priority ||
        (heap[smallest]->priority == heap[left]->priority &&
        strcmp(heap[left]->name, heap[smallest]->name) < 0))
        {
            smallest = left;
        }

    // If the right child is even smaller than the left child, we pseudo swap the right child with
    // the parent
    if (heap[smallest]->priority > heap[right]->priority ||
        (heap[smallest]->priority == heap[right]->priority &&
        strcmp(heap[right]->name, heap[smallest]->name) < 0))
        {
            smallest = right;
        }

    // We actually swap the two nodes here, and we go at it again, because the swap could've messed up
    // the conditions
    if (smallest != pos) {
        swap(heap[smallest], heap[pos]);
        min_heapify(heap, pos);
    }
}
/**
 * @brief Create a new sat that connect 2 other sats
 *
 * This function takes 2 sats and creates a third one that
 * connects the two of them
 * @param node1 The left node of the future new sat
 * @param node2 The right node of the future new sat
 */
node_t *combine_nodes(node_t *node1, node_t *node2)
{
    // Argument validation
    if (!node1 || !node2) {
        fprintf(stderr, "Cannot combine NULL nodes\n");
        return NULL;
    }

    // Alloc and sanity check
    node_t *new = create_node();
    if (!new) {
        return NULL;
    }

    // Create new node
    strcpy(new->name, node1->name);
    strcat(new->name, node2->name);
    new->priority = node1->priority + node2->priority;
    new->left = node1;
    new->right = node2;
    return new;
}


/**
 * @brief Recursively computes the size (number of nodes) of a binary tree
 *
 * @param node Pointer to the current node
 * @return Number of nodes in the subtree rooted at node
 */
static int __tree_size(node_t *node)
{
    if (!node) {
        return 0;
    }

    return 1 + __tree_size(node->left) + __tree_size(node->right);
}

/**
 * @brief Returns the size (number of nodes) of a binary tree
 *
 * @param tree Pointer to the binary tree
 * @return Number of nodes in the tree, or -1 if tree is NULL
 */
int tree_size(binary_tree_t *tree)
{
    if (!tree) {
        fprintf(stderr, "NULL tree given to the size function\n");
        return -1;
    }
    if (!tree->root) {
        return 0;
    }

    return __tree_size(tree->root);
}

/**
 * @brief Sorts an array of node_t pointers by priority and name.
 *
 * This function sorts the given heap array of node_t pointers in ascending order
 * based on the 'priority' field of each node. If two nodes have the same priority,
 * they are further sorted lexico by their 'name' field.
 *
 * @param heap An array of pointers to node_t structures to be sorted.
 * @param no_sat The number of elements in the heap array.
 */
static void sort_by_priority(node_t **heap, int no_sat)
{
    for (int i = 0; i < no_sat - 1; i++) {
        for (int j = i + 1; j < no_sat; j++) {
            if (heap[i]->priority > heap[j]->priority) {
                swap(heap[i], heap[j]);
            } else if (heap[i]->priority == heap[j]->priority) {
                if (strcmp(heap[i]->name, heap[j]->name) > 0) {
                    swap(heap[i], heap[j]);
                }
            }
        }
    }
}
/**
 * @brief Creates additional linking satellites and build the tre
 *
 * To build this tree-like network, a Min-Heap structure will be used to extract, at each step,
 * the satellites with the lowest reporting frequencies.
 * The satellite with the smaller reporting frequency will be placed as the left child of the connection node.
 * @param tree Pointer to the start of the satellite network
 * @param minheap The place from where we get the nodes
 * @param no_sat Number of satellites
 */
void construct_tree(binary_tree_t *tree, node_t **minheap, int no_sat)
{
    if (no_sat <= 1) {
        tree->root = minheap[0];
        return;
    }

    // We bring the lowest priorities to the left
    sort_by_priority(minheap, no_sat);

    // We heapify again
    // for (int i = no_sat / 2 - 1; i >= 0; i--) {
    //     min_heapify(minheap, i);
    // }

    // We combine the two lowest priority elements
    node_t *new = combine_nodes(minheap[0], minheap[1]);

    // We set the resulting node as root for now
    tree->root = new;
    // We replace the 2 combined nodes with the new node
    minheap[1] = new;
    // We have one less element in the minheap npw
    no_sat--;
    // We move all the elements to the right by 1
    for (int i = 0; i < no_sat; i++) {
        minheap[i] = minheap[i + 1];
    }
    // We repeat the process until the number of sats is 1
    construct_tree(tree, minheap, no_sat);
    // Get the size of the tree
    tree->size = tree_size(tree);
    return;
}

/**
 * @brief Reads the satellites and builds the network
 *
 * Reads a list of satellites (reporting frequency + name) and builds the network
 * Huffman-style: the two lowest-frequency nodes are repeatedly merged under a new
 * connection node, so frequently reporting satellites end up closer to the root.
 *
 * The lower-frequency node becomes the left child; ties are broken alphabetically.
 * A connection node's frequency is the sum of its children's and its name is their
 * names concatenated.
 * @param input_file Input file
 */

binary_tree_t *build_network(FILE *input_file)
{
    // Sanity checks and mallocs
    int no_sat;
    if (fscanf(input_file, "%d", &no_sat) != 1) {
        fprintf(stderr, "Error reading number of satellites\n");
        return NULL;
    }

    if (no_sat < 1) {
        fprintf(stderr, "Invalid number of sats given\n");
        return NULL;
    }

    node_t **minheap = create_node_array(no_sat);
    if (!minheap) {
        return NULL;
    }

    binary_tree_t *tree = create_binary_tree();
    if (!tree) {
        return NULL;
    }

    // Read loop
    // We read the satellites in the minheap array (not a minheap yet) and then we heapify it in the construct_tree function
    for (int i = 0; i < no_sat; i++) {
        if (fscanf(input_file, "%d %s", &minheap[i]->priority, minheap[i]->name) != 2) {
            fprintf(stderr, "Error reading satellite information\n");
            return NULL;
        }
    }

    // We now have all the sats saved in the node_t array
    // We contruct the tree now
    construct_tree(tree, minheap, no_sat);

    // We free the node_t * array because, when we free the tree, all the nodes from the array will be automatically freed
    for (int i = 0; i < no_sat; i++) {
        minheap[i] = NULL;
    }

    free(minheap);
    minheap = NULL;
    return tree;
}