#ifndef DATATYPES_H
#define DATATYPES_H

// Represent a pixel
typedef struct pixel {
    unsigned char red, green, blue;
} TPixel;

// QuadTree node
typedef struct quadTree {
    TPixel data;                // Pixel data
    unsigned char type;         // Node type (0 for internal, 1 for leaf)
    struct quadTree *kids[4];   // Pointers to child nodes
} TQuadTree;

// Queue node to store values from the file
typedef struct queue {
    TPixel data;                // Pixel data
    unsigned char type;         // Node type (0 for internal, 1 for leaf)
    struct queue *next;         // Pointer to the next node in the queue
} TQueue;

// Queue node that holds pointers to the elements in the quadtree
typedef struct parentsQueue {
    TQuadTree *elem;            // Pointer to a QuadTree node
    struct parentsQueue *next;  // Pointer to the next node in the parents queue
} TParentsQueue;

#endif // DATATYPES_H