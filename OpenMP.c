#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

#define MAX_TREE_HEIGHT 100

// Node structure for Huffman tree
struct Node {
    char data;
    int freq;
    struct Node* left;
    struct Node* right;
};

// Structure for Huffman code
struct Code {
    char symbol;
    char code[MAX_TREE_HEIGHT];
};

// Function to create a new node
struct Node* newNode(char data, int freq) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    if (node == NULL) {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    node->data = data;
    node->freq = freq;
    node->left = node->right = NULL;
    return node;
}

// Function to compare nodes based on frequency
int compareNodes(const void* a, const void* b) {
    struct Node* l = *((struct Node**)a);
    struct Node* r = *((struct Node**)b);
    return (l->freq > r->freq) - (l->freq < r->freq);
}

// Function to generate Huffman codes
void generateCodes(struct Node* root, char* code, int depth, struct Code* huffmanCode) {
    if (root == NULL)
        return;

    // If leaf node, store the code
    if (root->left == NULL && root->right == NULL) {
        huffmanCode[root->data].symbol = root->data;
        strcpy(huffmanCode[root->data].code, code);
    }

    // Traverse left and right, appending '0' and '1' respectively
    code[depth] = '0';
    generateCodes(root->left, code, depth + 1, huffmanCode);
    code[depth] = '1';
    generateCodes(root->right, code, depth + 1, huffmanCode);
}

// Function to perform Huffman coding
struct Code* huffmanCoding(char* input) {
    printf("Starting Huffman coding\n");

    int freq[256] = {0};

    // Count frequencies in parallel
    #pragma omp parallel for
    for (int i = 0; i < strlen(input); i++) {
        #pragma omp atomic
        freq[(int)input[i]]++;
    }

    printf("Frequency calculation completed\n");

    // Create nodes for characters with non-zero frequency
    struct Node* nodes[256];
    int num_nodes = 0;

    #pragma omp parallel for
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            nodes[i] = newNode((char)i, freq[i]);
            #pragma omp atomic
            num_nodes++;
        } else {
            nodes[i] = NULL;
        }
    }

    printf("Node creation completed\n");

    // Build Huffman tree
    while (num_nodes > 1) {
        struct Node* left = NULL;
        struct Node* right = NULL;

        // Find two nodes with the lowest frequencies
        #pragma omp parallel for
        for (int i = 0; i < 256; i++) {
            if (nodes[i] != NULL) {
                #pragma omp critical
                {
                    if (left == NULL || (nodes[i]->freq < left->freq && right == NULL)) {
                        left = nodes[i];
                    } else if (right == NULL || nodes[i]->freq < right->freq) {
                        right = nodes[i];
                    }
                }
            }
        }

        // Create parent node with combined frequency
        struct Node* top = newNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;

        // Set one of the min nodes to NULL
        #pragma omp critical
        {
            num_nodes--;
            left = top;
            right = NULL;
        }
    }

    printf("Huffman tree construction completed\n");

    // Generate Huffman codes
    struct Code* huffmanCode = (struct Code*)malloc(sizeof(struct Code) * 256);
    char code[MAX_TREE_HEIGHT];
    generateCodes(nodes[0], code, 0, huffmanCode);

    printf("Huffman code generation completed\n");

    return huffmanCode;
}

// Function to free memory allocated for Huffman codes
void freeHuffmanCode(struct Code* huffmanCode) {
    free(huffmanCode);
}

int main() {
    char* input = "hello world";
    double start = omp_get_wtime(); 
    struct Code* compressed = huffmanCoding(input);
    double end = omp_get_wtime();
    double elapsed = end - start;

    printf("Time taken: %f seconds\n", elapsed);

    freeHuffmanCode(compressed);

    return 0;
}
