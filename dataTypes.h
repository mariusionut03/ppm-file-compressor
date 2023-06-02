/* BUZOI Marius-Ionut */

// Represent a pixel

typedef struct pixel {
    unsigned char red, green, blue;
} TPixel;

// QuadTree

typedef struct quadTree {
    TPixel data;
    unsigned char type;
    struct quadTree *kids[4];
} TQuadTree;

// Queue where we will store the values from the file

typedef struct queue {
    TPixel data;
    unsigned char type;
    struct queue *next;
} TQueue;

// Queue that holds pointers to the elements in the quadtree

typedef struct parentsQueue {
    TQuadTree *elem;
    struct parentsQueue *next;
} TParentsQueue;