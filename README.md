# satnet — Satellite Network as a Huffman-style Binary Tree

A small C program that models a satellite network as a binary tree and runs a few
classic tree algorithms on it: Huffman-style construction, level-order traversal,
path encoding/decoding and lowest-common-ancestor lookup.

Satellites that report more often sit closer to the root, so their binary paths
(0 = left, 1 = right) are shorter, the same idea as Huffman coding.

Written in plain C99 with manual memory management; every run is Valgrind-clean.
Originally built for the Data Structures & Algorithms course at UPB.

## Features

| Flag  | Operation | What it does |
|-------|-----------|--------------|
| `-c1` | **Build & print** | Builds the network and prints it level by level (BFS) |
| `-c2` | **Decode** | Turns binary paths into the satellites they lead to |
| `-c3` | **Encode** | Turns satellite names into their concatenated binary paths |
| `-c4` | **Common ancestor** | Finds the closest node connecting a group of satellites |

## How it works

### Building the network
Each satellite has a name and a reporting frequency. The tree is built bottom-up:

1. Sort all nodes by frequency (ties broken alphabetically).
2. Take the two lowest, and join them under a new *connection node* whose frequency
   is the sum of both and whose name is their names concatenated. The lower one goes left.
3. Put the new node back and repeat until a single root is left.

For the input in [`examples/network.txt`](examples/network.txt):

```
6
5 A
9 B
12 C
13 D
16 E
45 F
```

the resulting tree (`-c1` output, one level per line) is:

```
100-FCDABE
45-F 55-CDABE
25-CD 30-ABE
12-C 13-D 14-AB 16-E
5-A 9-B
```

```
              100 FCDABE
             /          \
          45 F        55 CDABE
                     /        \
                 25 CD        30 ABE
                 /   \        /    \
              12 C  13 D   14 AB   16 E
                           /   \
                         5 A   9 B
```

### Decoding (`-c2`)
Walks the tree following the bits. When a leaf is reached the satellite is printed and
the walk restarts from the root, so one code can hold several satellites.
`0101100` → `F D C`, `11001101` → `A B`.

### Encoding (`-c3`)
The reverse: each name is turned into its root-to-leaf path and the paths are
concatenated. `F A E` → `0` + `1100` + `111` = `01100111`.

### Lowest common ancestor (`-c4`)
Every satellite is encoded as a path; the longest common prefix of all those paths
leads straight to their closest common ancestor. `A B E` → `ABE`.

## Project structure

```
.
├── include/
│   ├── tree.h        # node_t / binary_tree_t and tree utilities
│   ├── builder.h     # network construction
│   ├── decoder.h     # path -> satellite decoding
│   ├── encoder.h     # satellite -> path encoding
│   └── ancestor.h    # lowest common ancestor
├── src/
│   ├── main.c        # CLI entry point
│   ├── tree.c        # allocation, freeing, BFS printing
│   ├── builder.c     # sorting, node merging, tree construction
│   ├── decoder.c
│   ├── encoder.c
│   └── ancestor.c
├── examples/         # sample inputs for each mode
└── Makefile
```

### Core data structures

```c
typedef struct node_t {
    int priority;          // reporting frequency (sum of children for connection nodes)
    char *name;            // satellite name, or concatenated child names
    struct node_t *left;
    struct node_t *right;
} node_t;

typedef struct binary_tree_t {
    node_t *root;
    int size;              // total number of nodes
} binary_tree_t;
```

Leaves are real satellites; internal nodes are the connections between them.

## Build & run

Requires `gcc` and `make` (Linux, macOS or WSL).

```bash
make
```

```bash
./satnet <-c1|-c2|-c3|-c4> <input_file> <output_file>
```

Example:

```bash
./satnet -c2 examples/decode.txt out.txt && cat out.txt
```

### Input format
Every input starts with the network definition:

```
N
<frequency> <name>    # repeated N times
```

For `-c2`, `-c3` and `-c4` it is followed by a count and that many entries
(binary codes for `-c2`, satellite names for `-c3` / `-c4`).
See [`examples/`](examples/) for one file per mode.

### Memory checking

```bash
valgrind --leak-check=full ./satnet -c1 examples/network.txt out.txt
```

## Author

Cosmin-George Baroană
