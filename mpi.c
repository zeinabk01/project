#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mpi.h>

#define MAX_TREE_HEIGHT 100

// Node structure for Huffman tree
struct Node {
    char data;
    int frequency;
    struct Node *left, *right;
};

// Structure for Huffman code
struct Code {
    char symbol;
    char code[MAX_TREE_HEIGHT];
};

// Function to create a new node
struct Node* newNode(char data, int frequency) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    if (node == NULL) {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->frequency = frequency;
    node->left = node->right = NULL;
    return node;
}

// Function to generate Huffman codes
void generateCodes(struct Node* root, char* code, int depth, struct Code* codes) {
    if (!root) {
        return;
    }
    if (root->data != '\0') {
        codes[root->data].symbol = root->data;
        strcpy(codes[root->data].code, code);
    }
    code[depth] = '0';
    generateCodes(root->left, code, depth + 1, codes);
    code[depth] = '1';
    generateCodes(root->right, code, depth + 1, codes);
}

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double start_time, end_time;

    if (rank == 0) {
        start_time = MPI_Wtime();
    }

    char input[] = "hello world";

    int inputSize = strlen(input);
    int bufferSize = inputSize / size + (inputSize % size != 0 ? 1 : 0);

    char buffer[bufferSize];
    MPI_Scatter(input, bufferSize, MPI_CHAR, buffer, bufferSize, MPI_CHAR, 0, MPI_COMM_WORLD);

    int freq[256] = {0};
    for (int i = 0; i < bufferSize; i++) {
        freq[(int)buffer[i]]++;
    }

    int globalFreq[256] = {0};
    MPI_Reduce(freq, globalFreq, 256, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    struct Node* nodes[256];
    int num_nodes = 0;

    for (int i = 0; i < 256; i++) {
        if (globalFreq[i] > 0) {
            nodes[i] = newNode((char)i, globalFreq[i]);
            num_nodes++;
        } else {
            nodes[i] = NULL;
        }
    }

    while (num_nodes > 1) {
        struct Node* left = NULL;
        struct Node* right = NULL;

        for (int i = 0; i < 256; i++) {
            if (nodes[i] != NULL) {
                if (left == NULL || (nodes[i]->frequency < left->frequency && right == NULL)) {
                    left = nodes[i];
                } else if (right == NULL || nodes[i]->frequency < right->frequency) {
                    right = nodes[i];
                }
            }
        }

        struct Node* top = newNode('$', left->frequency + right->frequency);
        top->left = left;
        top->right = right;

        num_nodes--;
        left = top;
        right = NULL;
    }

    struct Code* codes = (struct Code*)malloc(sizeof(struct Code) * 256);
    char code[MAX_TREE_HEIGHT];
    generateCodes(nodes[0], code, 0, codes);

    if (rank == 0) {
        for (int i = 0; i < size; i++) {
            if (i != rank) {
                MPI_Send(codes, 256 * 2, MPI_CHAR, i, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        MPI_Recv(codes, 256 * 2, MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    char encoded[bufferSize * MAX_TREE_HEIGHT];
    int index = 0;
    for (int i = 0; i < bufferSize; i++) {
        struct Code c = codes[(int)buffer[i]];
        strcpy(encoded + index, c.code);
        index += strlen(c.code);
    }

    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("Parallel execution time: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();

    return 0;
}
