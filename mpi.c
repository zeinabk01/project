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

    double start_time_seq, end_time_seq, start_time_par, end_time_par;

    // Sequential Execution Time
    if (rank == 0) {
        start_time_seq = MPI_Wtime();
    }

    // Increase the input data length
    const char* data = "The ability to scale your tables in terms of both storage and provision throughput makes DynamoDB a good fit for structured data from the web, mobile, and IoT applications. For instance, you might have a large number of clients that continuously generate data and make large numbers of requests per second. In this case, the throughput scaling of DynamoDB enables consistent performance for your clients. DynamoDB is also used in latency-sensitive applications. The predictable query performance—even in large tables—makes it useful for cases where variable latency could cause significant impact to the user experience or to business goals, such as adtech or gaminghdffwgtueqyuiwpdcnbvvcsed ex at quam elementum convallis. Aliquam nec turpis eget ligula elementum faucibus. Aliquam id diam orci. Phasellus id augue euismod, mollis lacus ut, vehicula tortor. Morbi eleifend lorem at velit dapibus tempus. Nullam fermentum, felis id convallis vestibulum, nunc arcu varius leo, a dictum magna nisi ac ex. Nam vehicula, sapien eget eleifend commodo, nibh orci accumsan tortor, non cursus elit metus nec nulla. Sed ornare tincidunt elit, in efficitur urna feugiat sed. Sed nec urna ac nulla vestibulum pellentesque. Sed in suscipit orci, ac tristique lorem. In id enim quis arcu lobortis malesuada eget eget lorem. Morbi vel velit eu nunc vehicula blandit ac vel dui. Nulla vitae libero id lacus fermentum rutrum ac id nisi. In at tortor quis nunc tempor consequat nec sed sem. Cras sit amet ex et sapien suscipit scelerisque. Sed non lacus ut nunc ultrices iaculis. Donec ac luctus nisi, ac ultrices purus. Morbi tincidunt lectus eget sapien consequat placerat. Ut ac dui ut lectus vehicula mattis id a dui. Sed pellentesque vestibulum feugiat. Proin rhoncus ullamcorper odio, ac vulputate libero faucibus at. Etiam posuere lobortis odio, et eleifend ligula suscipit id. Nam auctor placerat turpis, in ullamcorper est aliquam quis. Integer nec magna ut velit venenatis laoreet. Nam convallis, magna nec viverra tempor, quam quam iaculis felis, vel rhoncus libero odio id sapien. Etiam vestibulum feugiat ante. Nullam cursus, leo eget convallis sollicitudin, nisi dolor eleifend urna, euismod finibus justo mauris eget felis. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posu. The world of technology is ever-evolving, with new advancements and innovations constantly shaping the way we live and work. From artificial intelligence and machine learning to virtual reality and blockchain technology, the possibilities seem endless. These advancements have the potential to revolutionize industries, from healthcare and finance to transportation and entertainment. As we continue to push the boundaries of what's possible, it's crucial to embrace these changes and adapt to the fast-paced digital landscape. By staying informed and open to new ideas, we can harness the power of technology to create a brighter future for generations to come. In the vast landscape of human endeavor, technology stands as a towering force, shaping the course of history and driving the evolution of society. With each passing day, new breakthroughs emerge, pushing the boundaries of innovation and challenging our understanding of what's possible. From the transformative potential of artificial intelligence to the disruptive power of blockchain, the digital frontier is ablaze with possibility. In fields as diverse as healthcare, finance, transportation, and entertainment, technology is revolutionizing the way we live, work, and interact with the world around us. Yet amidst this whirlwind of change, one thing remains constant: the human spirit of curiosity and exploration. As we journey deeper into the digital age, let us embrace the challenges and opportunities that lie ahead, harnessing the power of technology to build a brighter, more inclusive future for all. The ability to scale your tables in terms of both storage and provision throughput makes DynamoDB a good fit for structured data from the web, mobile, and IoT applications. For instance, you might have a large number of clients that continuously generate data and make large numbers of requests per second. In this case, the throughput scaling of DynamoDB enables consistent performance for your clients. DynamoDB is also used in latency-sensitive applications. The predictable query performance—even in large tables—makes it useful for cases where variable latency could cause significant impact to the user experience or to business goals, such as adtech or gaminghdffwgtueqyuiwpdcnbvvcsed ex at quam elementum convallis. Aliquam nec turpis eget ligula elementum faucibus. Aliquam id diam orci. Phasellus id augue euismod, mollis lacus ut, vehicula tortor. Morbi eleifend lorem at velit dapibus tempus. Nullam fermentum, felis id convallis vestibulum, nunc arcu varius leo, a dictum magna nisi ac ex. Nam vehicula, sapien eget eleifend commodo, nibh orci accumsan tortor, non cursus elit metus nec nulla. Sed ornare tincidunt elit, in efficitur urna feugiat sed. Sed nec urna ac nulla vestibulum pellentesque. Sed in suscipit orci, ac tristique lorem. In id enim quis arcu lobortis malesuada eget eget lorem. Morbi vel velit eu nunc vehicula blandit ac vel dui. Nulla vitae libero id lacus fermentum rutrum ac id nisi. In at tortor quis nunc tempor consequat nec sed sem. Cras sit amet ex et sapien suscipit scelerisque. Sed non lacus ut nunc ultrices iaculis. Donec ac luctus nisi, ac ultrices purus. Morbi tincidunt lectus eget sapien consequat placerat. Ut ac dui ut lectus vehicula mattis id a dui. Sed pellentesque vestibulum feugiat. Proin rhoncus ullamcorper odio, ac vulputate libero faucibus at. Etiam posuere lobortis odio, et eleifend ligula suscipit id. Nam auctor placerat turpis, in ullamcorper est aliquam quis. Integer nec magna ut velit venenatis laoreet. Nam convallis, magna nec viverra tempor, quam quam iaculis felis, vel rhoncus libero odio id sapien. Etiam vestibulum feugiat ante. Nullam cursus, leo eget convallis sollicitudin, nisi dolor eleifend urna, euismod finibus justo mauris eget felis. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posu. The world of technology is ever-evolving, with new advancements and innovations constantly shaping the way we live and work. From artificial intelligence and machine learning to virtual reality and blockchain technology, the possibilities seem endless. These advancements have the potential to revolutionize industries, from healthcare and finance to transportation and entertainment. As we continue to push the boundaries of what's possible, it's crucial to embrace these changes and adapt to the fast-paced digital landscape. By staying informed and open to new ideas, we can harness the power of technology to create a brighter future for generations to come. In the vast landscape of human endeavor, technology stands as a towering force, shaping the course of history and driving the evolution of society. With each passing day, new breakthroughs emerge, pushing the boundaries of innovation and challenging our understanding of what's possible. From the transformative potential of artificial intelligence to the disruptive power of blockchain, the digital frontier is ablaze with possibility. In fields as diverse as healthcare, finance, transportation, and entertainment, technology is revolutionizing the way we live, work, and interact with the world around us. Yet amidst this whirlwind of change, one thing remains constant: the human spirit of curiosity and exploration. As we journey deeper into the digital age, let us embrace the challenges and opportunities that lie ahead, harnessing the power of technology to build a brighter, more inclusive future for all. The ability to scale your tables in terms of both storage and provision throughput makes DynamoDB a good fit for structured data from the web, mobile, and IoT applications. For instance, you might have a large number of clients that continuously generate data and make large numbers of requests per second. In this case, the throughput scaling of DynamoDB enables consistent performance for your clients. DynamoDB is also used in latency-sensitive applications. The predictable query performance—even in large tables—makes it useful for cases where variable latency could cause significant impact to the user experience or to business goals, such as adtech or gaminghdffwgtueqyuiwpdcnbvvcsed ex at quam elementum convallis. Aliquam nec turpis eget ligula elementum faucibus. Aliquam id diam orci. Phasellus id augue euismod, mollis lacus ut, vehicula tortor. Morbi eleifend lorem at velit dapibus tempus. Nullam fermentum, felis id convallis vestibulum, nunc arcu varius leo, a dictum magna nisi ac ex. Nam vehicula, sapien eget eleifend commodo, nibh orci accumsan tortor, non cursus elit metus nec nulla. Sed ornare tincidunt elit, in efficitur urna feugiat sed. Sed nec urna ac nulla vestibulum pellentesque. Sed in suscipit orci, ac tristique lorem. In id enim quis arcu lobortis malesuada eget eget lorem. Morbi vel velit eu nunc vehicula blandit ac vel dui. Nulla vitae libero id lacus fermentum rutrum ac id nisi. In at tortor quis nunc tempor consequat nec sed sem. Cras sit amet ex et sapien suscipit scelerisque. Sed non lacus ut nunc ultrices iaculis. Donec ac luctus nisi, ac ultrices purus. Morbi tincidunt lectus eget sapien consequat placerat. Ut ac dui ut lectus vehicula mattis id a dui. Sed pellentesque vestibulum feugiat. Proin rhoncus ullamcorper odio, ac vulputate libero faucibus at. Etiam posuere lobortis odio, et eleifend ligula suscipit id. Nam auctor placerat turpis, in ullamcorper est aliquam quis. Integer nec magna ut velit venenatis laoreet. Nam convallis, magna nec viverra tempor, quam quam iaculis felis, vel rhoncus libero odio id sapien. Etiam vestibulum feugiat ante. Nullam cursus, leo eget convallis sollicitudin, nisi dolor eleifend urna, euismod finibus justo mauris eget felis. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posu. The world of technology is ever-evolving, with new advancements and innovations constantly shaping the way we live and work. From artificial intelligence and machine learning to virtual reality and blockchain technology, the possibilities seem endless. These advancements have the potential to revolutionize industries, from healthcare and finance to transportation and entertainment. As we continue to push the boundaries of what's possible, it's crucial to embrace these changes and adapt to the fast-paced digital landscape. By staying informed and open to new ideas, we can harness the power of technology to create a brighter future for generations to come. In the vast landscape of human endeavor, technology stands as a towering force, shaping the course of history and driving the evolution of society. With each passing day, new breakthroughs emerge, pushing the boundaries of innovation and challenging our understanding of what's possible. From the transformative potential of artificial intelligence to the disruptive power of blockchain, the digital frontier is ablaze with possibility. In fields as diverse as healthcare, finance, transportation, and entertainment, technology is revolutionizing the way we live, work, and interact with the world around us. Yet amidst this whirlwind of change, one thing remains constant: the human spirit of curiosity and exploration. As we journey deeper into the digital age, let us embrace the challenges and opportunities that lie ahead, harnessing the power of technology to build a brighter, more inclusive future for all.The ability to scale your tables in terms of both storage and provision throughput makes DynamoDB a good fit for structured data from the web, mobile, and IoT applications. For instance, you might have a large number of clients that continuously generate data and make large numbers of requests per second. In this case, the throughput scaling of DynamoDB enables consistent performance for your clients. DynamoDB is also used in latency-sensitive applications. The predictable query performance—even in large tables—makes it useful for cases where variable latency could cause significant impact to the user experience or to business goals, such as adtech or gaminghdffwgtueqyuiwpdcnbvvcsed ex at quam elementum convallis. Aliquam nec turpis eget ligula elementum faucibus. Aliquam id diam orci. Phasellus id augue euismod, mollis lacus ut, vehicula tortor. Morbi eleifend lorem at velit dapibus tempus. Nullam fermentum, felis id convallis vestibulum, nunc arcu varius leo, a dictum magna nisi ac ex. Nam vehicula, sapien eget eleifend commodo, nibh orci accumsan tortor, non cursus elit metus nec nulla. Sed ornare tincidunt elit, in efficitur urna feugiat sed. Sed nec urna ac nulla vestibulum pellentesque. Sed in suscipit orci, ac tristique lorem. In id enim quis arcu lobortis malesuada eget eget lorem. Morbi vel velit eu nunc vehicula blandit ac vel dui. Nulla vitae libero id lacus fermentum rutrum ac id nisi. In at tortor quis nunc tempor consequat nec sed sem. Cras sit amet ex et sapien suscipit scelerisque. Sed non lacus ut nunc ultrices iaculis. Donec ac luctus nisi, ac ultrices purus. Morbi tincidunt lectus eget sapien consequat placerat. Ut ac dui ut lectus vehicula mattis id a dui. Sed pellentesque vestibulum feugiat. Proin rhoncus ullamcorper odio, ac vulputate libero faucibus at. Etiam posuere lobortis odio, et eleifend ligula suscipit id. Nam auctor placerat turpis, in ullamcorper est aliquam quis. Integer nec magna ut velit venenatis laoreet. Nam convallis, magna nec viverra tempor, quam quam iaculis felis, vel rhoncus libero odio id sapien. Etiam vestibulum feugiat ante. Nullam cursus, leo eget convallis sollicitudin, nisi dolor eleifend urna, euismod finibus justo mauris eget felis. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posu. The world of technology is ever-evolving, with new advancements and innovations constantly shaping the way we live and work. From artificial intelligence and machine learning to virtual reality and blockchain technology, the possibilities seem endless. These advancements have the potential to revolutionize industries, from healthcare and finance to transportation and entertainment. As we continue to push the boundaries of what's possible, it's crucial to embrace these changes and adapt to the fast-paced digital landscape. By staying informed and open to new ideas, we can harness the power of technology to create a brighter future for generations to come. In the vast landscape of human endeavor, technology stands as a towering force, shaping the course of history and driving the evolution of society. With each passing day, new breakthroughs emerge, pushing the boundaries of innovation and challenging our understanding of what's possible. From the transformative potential of artificial intelligence to the disruptive power of blockchain, the digital frontier is ablaze with possibility. In fields as diverse as healthcare, finance, transportation, and entertainment, technology is revolutionizing the way we live, work, and interact with the world around us. Yet amidst this whirlwind of change, one thing remains constant: the human spirit of curiosity and exploration. As we journey deeper into the digital age, let us embrace the challenges and opportunities that lie ahead, harnessing the power of technology to build a brighter, more inclusive future for all. orci, ac tristique lorem. In id enim quis arcu lobortis malesuada eget eget lorem. Morbi vel velit eu nunc vehicula blandit ac vel dui. Nulla vitae libero id lacus fermentum rutrum ac id nisi. In at tortor quis nunc tempor consequat nec sed sem. Cras sit amet ex et sapien suscipit scelerisque. Sed non lacus ut nunc ultrices iaculis. Donec ac luctus nisi, ac ultrices purus. Morbi tincidunt lectus eget sapien consequat placerat. Ut ac dui ut lectus vehicula mattis id a dui. Sed pellentesque vestibulum feugiat. Proin rhoncus ullamcorper odio, ac vulputate libero faucibus at. Etiam posuere lobortis odio, et eleifend ligula suscipit id. Nam auctor placerat turpis, in ullamcorper est aliquam quis. Integer nec magna ut velit venenatis laoreet. Nam convallis, magna nec viverra tempor, quam quam iaculis felis, vel rhoncus libero odio id sapien. Etiam vestibulum feugiat ante. Nullam cursus, leo eget convallis sollicitudin, nisi dolor eleifend urna, euismod finibus justo mauris eget felis. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posu. The world of technology is ever-evolving, with new advancements and innovations constantly shaping the way we live and work. From artificial intelligence and machine learning to virtual reality and blockchain technology, the possibilities seem endless. These advancements have the potential to revolutionize industries, from healthcare and finance to transportation and entertainment. As we continue to push the boundaries of what's possible, it's crucial to embrace these changes and adapt to the fast-paced digital landscape. By staying informed and open to new ideas, we can harness the power of technology to create a brighter future for generations to come. In the vast landscape of human endeavor, technology stands as a towering force, shaping the course of history and driving the evolution of society. With each passing day, new breakthroughs emerge, pushing the boundaries of innovation and challenging our understanding of what's possible. From the transformative potential of artificial intelligence to the disruptive power of blockchain, the digital frontier is ablaze with possibility. In fields as diverse as healthcare, finance, transportation, and entertainment, technology is revolutionizing the way we live, work, and interact with the world around us. Yet amidst this whirlwind of change, one thing remains constant: the human spirit of curiosity and exploration. As we journey deeper into the digital age, let us embrace the challenges and opportunities that lie ahead, harnessing the power of technology to build a brighter, more inclusive future for all.The ability to scale your tables in terms of both storage and provision throughput makes DynamoDB a good fit for structured data from the web, mobile, and IoT applications. For instance, you might have a large number of clients that continuously generate data and make large numbers of requests per second. In this case, the throughput scaling of DynamoDB enables consistent performance for your clients. DynamoDB is also used in latency-sensitive applications. The predictable query performance—even in large tables—makes it useful for cases where variable latency could cause significant impact to the user experience or to business goals, such as adtech or gaminghdffwgtueqyuiwpdcnbvvcsed ex at quam elementum convallis. Aliquam nec turpis eget ligula elementum faucibus. Aliquam id diam orci. Phasellus id augue euismod, mollis lacus ut, vehicula tortor. Morbi eleifend lorem at velit dapibus tempus. Nullam fermentum, felis id convallis vestibulum, nunc arcu varius leo, a dictum magna nisi ac ex. Nam vehicula, sapien eget eleifend commodo, nibh orci accumsan tortor, non cursus elit metus nec nulla. Sed ornare tincidunt elit, in efficitur urna feugiat sed. Sed nec urna ac nulla vestibulum pellentesque. Sed in suscipit orci, ac tristique lorem. In id enim quis arcu lobortis malesuada eget eget lorem. Morbi vel velit eu nunc vehicula blandit ac vel dui. Nulla vitae libero id lacus fermentum rutrum ac id nisi. In at tortor quis nunc tempor consequat nec sed sem. Cras sit amet ex et sapien suscipit scelerisque. Sed non lacus ut nunc ultrices iaculis. Donec ac luctus nisi, ac ultrices purus. Morbi tincidunt lectus eget sapien consequat placerat. Ut ac dui ut lectus vehicula mattis id a dui. Sed pellentesque vestibulum feugiat. Proin rhoncus ullamcorper odio, ac vulputate libero faucibus at. Etiam posuere lobortis odio, et eleifend ligula suscipit id. Nam auctor placerat turpis, in ullamcorper est aliquam quis. Integer nec magna ut velit venenatis laoreet. Nam convallis, magna nec viverra tempor, quam quam iaculis felis, vel rhoncus libero odio id sapien. Etiam vestibulum feugiat ante. Nullam cursus, leo eget convallis sollicitudin, nisi dolor eleifend urna, euismod finibus justo mauris eget felis. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posu. The world of technology is ever-evolving, with new advancements and innovations constantly shaping the way we live and work. From artificial intelligence and machine learning to virtual reality and blockchain technology, the possibilities seem endless. These advancements have the potential to revolutionize industries, from healthcare and finance to transportation and entertainment. As we continue to push the boundaries of what's possible, it's crucial to embrace these changes and adapt to the fast-paced digital landscape. By staying informed and open to new ideas, we can harness the power of technology to create a brighter future for generations to come. In the vast landscape of human endeavor, technology stands as a towering force, shaping the course of history and driving the evolution of society. With each passing day, new breakthroughs emerge, pushing the boundaries of innovation and challenging our understanding of what's possible. From the transformative potential of artificial intelligence to the disruptive power of blockchain, the digital frontier is ablaze with possibility. In fields as diverse as healthcare, finance, transportation, and entertainment, technology is revolutionizing the way we live, work, and interact with the world around us. Yet amidst this whirlwind of change, one thing remains constant: the human spirit of curiosity and exploration. As we journey deeper into the digital age, let us embrace the challenges and opportunities that lie ahead, harnessing the power of technology to build a brighter, more inclusive future for ";

    int inputSize = strlen(data);
    int bufferSize = inputSize / size + (inputSize % size != 0 ? 1 : 0);

    char buffer[bufferSize];
    MPI_Scatter(data, bufferSize, MPI_CHAR, buffer, bufferSize, MPI_CHAR, 0, MPI_COMM_WORLD);

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

    // Parallel Execution Time
    if (rank == 0) {
        end_time_par = MPI_Wtime();
        printf("Parallel execution time: %f seconds\n", end_time_par - start_time_par);
    }

    MPI_Finalize();

    return 0;
}
