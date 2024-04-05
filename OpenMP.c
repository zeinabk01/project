#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

struct Node {
    char data;
    int freq;
    struct Node* left;
    struct Node* right;
};

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

int compareNodes(const void* a, const void* b) {
    struct Node* l = *((struct Node**)a);
    struct Node* r = *((struct Node**)b);
    return (l->freq > r->freq) - (l->freq < r->freq);
}

char** huffmanCoding(char* input) {
    printf("Starting Huffman coding\n");

    int freq[256] = {0};
    #pragma omp parallel for num_threads(2)
    for (int i = 0; i < strlen(input); i++) {
        #pragma omp atomic
        freq[(int)input[i]]++;
    }

    printf("Frequency calculation completed\n");

    struct Node* nodes[256];
    int num_nodes = 0;

    #pragma omp parallel for num_threads(2)
    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            nodes[i] = newNode((char)i, freq[i]);
        } else {
            nodes[i] = NULL;
        }
    }

    printf("Node creation completed\n");

    while (1) {
        // Find the two nodes with the smallest frequencies
        int min1 = -1, min2 = -1;
        #pragma omp parallel for num_threads(4)
        for (int i = 0; i < 256; i++) {
            if (nodes[i] != NULL) {
                #pragma omp critical
                {
                    if (min1 == -1 || nodes[i]->freq < nodes[min1]->freq) {
                        min2 = min1;
                        min1 = i;
                    } else if (min2 == -1 || nodes[i]->freq < nodes[min2]->freq) {
                        min2 = i;
                    }
                }
            }
        }

        // If only one node left, Huffman tree construction completed
        if (min2 == -1)
            break;

        // Create a new node with combined frequency
        struct Node* top = newNode('$', nodes[min1]->freq + nodes[min2]->freq);
        top->left = nodes[min1];
        top->right = nodes[min2];

        // Set one of the min nodes to NULL
        #pragma omp critical
        {
            nodes[min1] = top;
            nodes[min2] = NULL;
        }
    }

    printf("Huffman tree construction completed\n");

    char** huffmanCode = (char**)malloc(sizeof(char*) * 256);
    if (huffmanCode == NULL) {
        printf("Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < 256; i++) {
        huffmanCode[i] = (char*)malloc(256 * sizeof(char));
        if (huffmanCode[i] == NULL) {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        huffmanCode[i][0] = '\0'; // Initialize each string in the array
    }

    return huffmanCode;
}


void freeHuffmanCode(char** huffmanCode) {
    for (int i = 0; i < 256; i++) {
        free(huffmanCode[i]);
    }
    free(huffmanCode);
}

int main() {
    char* input = "zeinab";
    double start = omp_get_wtime(); 
    char** compressed = huffmanCoding(input);
    double end = omp_get_wtime();
    double elapsed = end - start;

    printf("Time taken: %f seconds\n", elapsed);

    freeHuffmanCode(compressed);

    return 0;
}
