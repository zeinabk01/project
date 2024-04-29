%%cu
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cuda.h>
#include <cuda_runtime.h>

#define MAX_SYMBOLS 256
#define MAX_INPUT_LENGTH 1000000
#define BLOCK_SIZE 1024

typedef struct {
    int symbol;
    int frequency;
} node;

typedef struct {
    int bit_length;
    int code;
} huffman_code;

_global_ void compute_frequencies(char* input, int input_length, node* frequencies) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < input_length) {
        atomicAdd(&(frequencies[input[tid]].frequency), 1);
    }
}

_global_ void construct_huffman_tree(node* frequencies, int num_symbols, int* parents, int* left_children, int* right_children) {
    int num_nodes = 2 * num_symbols - 1;
    for (int i = num_symbols; i < num_nodes; i++) {
        int min1_idx = -1, min2_idx = -1;
        for (int j = 0; j < i; j++) {
            if (parents[j] == -1) {
                if (min1_idx == -1 || frequencies[j].frequency < frequencies[min1_idx].frequency) {
                    min2_idx = min1_idx;
                    min1_idx = j;
                }
                else if (min2_idx == -1 || frequencies[j].frequency < frequencies[min2_idx].frequency) {
                    min2_idx = j;
                }
            }
        }
        parents[min1_idx] = i;
        left_children[i] = min1_idx;
        parents[min2_idx] = i;
        right_children[i] = min2_idx;
        frequencies[i].frequency = frequencies[min1_idx].frequency + frequencies[min2_idx].frequency;
    }
}

_global_ void compute_codes(int* parents, int* left_children, int* right_children, huffman_code* codes, int num_symbols) {
    int tid = blockIdx.x * blockDim.x + threadIdx.x;
    if (tid < num_symbols) {
        int node_idx = tid;
        int code = 0;
        int bit_length = 0;
        while (parents[node_idx] != -1) {
            int parent_idx = parents[node_idx];
            if (left_children[parent_idx] == node_idx) {
                code |= (1 << bit_length);
            }
            bit_length++;
            node_idx = parent_idx;
        }
        codes[tid].code = code;
        codes[tid].bit_length = bit_length;
    }
}

int main() {
    char input[] = "helloefdiwa";
    int input_length = 11;
    node* frequencies = (node*) calloc(MAX_SYMBOLS, sizeof(node));
    char* d_input;
    node* d_frequencies;
    cudaMalloc((void**) &d_input, input_length * sizeof(char));
    cudaMalloc((void**) &d_frequencies, MAX_SYMBOLS * sizeof(node));
    cudaMemcpy(d_input, input, input_length * sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(d_frequencies, frequencies, MAX_SYMBOLS * sizeof(node), cudaMemcpyHostToDevice);
    int num_blocks = (input_length + BLOCK_SIZE - 1) / BLOCK_SIZE;
    compute_frequencies<<<num_blocks, BLOCK_SIZE>>>(d_input, input_length, d_frequencies);
    cudaDeviceSynchronize();
    cudaMemcpy(frequencies, d_frequencies, MAX_SYMBOLS * sizeof(node), cudaMemcpyDeviceToHost);
    int num_symbols = 0;
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (frequencies[i].frequency > 0) {
            num_symbols++;
        }
    }
    int num_nodes = 2 * num_symbols - 1;
    int* parents = (int*) malloc(num_nodes * sizeof(int));
    int* left_children = (int*) malloc(num_nodes * sizeof(int));
    int* right_children = (int*) malloc(num_nodes * sizeof(int));
    for (int i = 0; i < num_nodes; i++) {
        parents[i] = -1;
        left_children[i] = -1;
        right_children[i] = -1;
    }
    for (int i = 0; i < MAX_SYMBOLS; i++) {
        if (frequencies[i].frequency > 0) {
            parents[i] = -2;
        }
    }
    int* d_parents;
    int* d_left_children;
    int* d_right_children;
    cudaMalloc((void**) &d_parents, num_nodes * sizeof(int));
    cudaMalloc((void**) &d_left_children, num_nodes * sizeof(int));
    cudaMalloc((void**) &d_right_children, num_nodes * sizeof(int));
    cudaMemcpy(d_parents, parents, num_nodes * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_left_children, left_children, num_nodes * sizeof(int), cudaMemcpyHostToDevice);
    cudaMemcpy(d_right_children, right_children, num_nodes * sizeof(int), cudaMemcpyHostToDevice);
    construct_huffman_tree<<<1, 1>>>(d_frequencies, num_symbols, d_parents, d_left_children, d_right_children);
    cudaDeviceSynchronize();
    cudaMemcpy(parents, d_parents, num_nodes * sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(left_children, d_left_children, num_nodes * sizeof(int), cudaMemcpyDeviceToHost);
    cudaMemcpy(right_children, d_right_children, num_nodes * sizeof(int), cudaMemcpyDeviceToHost);
    huffman_code* codes = (huffman_code*) malloc(num_symbols * sizeof(huffman_code));
    huffman_code* d_codes;
    cudaMalloc((void**) &d_codes, num_symbols * sizeof(huffman_code));
   

compute_codes<<<num_blocks, BLOCK_SIZE>>>(d_parents, d_left_children, d_right_children, d_codes, num_symbols);
cudaDeviceSynchronize();
cudaMemcpy(codes, d_codes, num_symbols * sizeof(huffman_code), cudaMemcpyDeviceToHost);
for (int i = 0; i < num_symbols; i++) {
printf("%c: %d %d\n", (char) i, codes[i].code, codes[i].bit_length);
}
cudaFree(d_input);
cudaFree(d_frequencies);
cudaFree(d_parents);
cudaFree(d_left_children);
cudaFree(d_right_children);
cudaFree(d_codes);
free(frequencies);
free(parents);
free(left_children);
free(right_children);
free(codes);
return 0;
}
