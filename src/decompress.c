#include "decompress.h"

/* Build a pixel matrix based on a quadtree */
void createPPMfile(TQuadTree *node, TPixel **mat, int rowStart, int rowEnd, int colStart, int colEnd) {
    if (node->kids[0] == NULL && node->kids[1] == NULL && node->kids[2] == NULL && node->kids[3] == NULL) {
        // Leaf node, fill the block with the values
        for (int i = rowStart; i < rowEnd; i++) {
            for (int j = colStart; j < colEnd; j++) {
                mat[i][j].red = node->data.red;
                mat[i][j].green = node->data.green;
                mat[i][j].blue = node->data.blue;
            }
        }
    } else {
        // Internal node, divide again
        int midrow = (rowStart + rowEnd) / 2;
        int midcol = (colStart + colEnd) / 2;
        createPPMfile(node->kids[0], mat, rowStart, midrow, colStart, midcol);
        createPPMfile(node->kids[1], mat, rowStart, midrow, midcol, colEnd);
        createPPMfile(node->kids[2], mat, midrow, rowEnd, midcol, colEnd);
        createPPMfile(node->kids[3], mat, midrow, rowEnd, colStart, midcol);
    }
}

/* Build a quadtree based on the values from the file containing the compressed image */
void buildTheQuadTreeFromQueue(TParentsQueue *parentsQueue, TQueue *queue) {
    // Initialize the children of the current node
    for (int i = 0; i < 4; i++) {
        parentsQueue->elem->kids[i] = malloc(sizeof(TQuadTree));
        parentsQueue->elem->kids[i]->data.red = parentsQueue->elem->kids[i]->data.green = parentsQueue->elem->kids[i]->data.blue = 0;
        for (int j = 0; j < 4; j++) {
            parentsQueue->elem->kids[i]->kids[j] = NULL;
        }
    }

    // Process each child
    for (int i = 0; i < 4; i++) {
        if (queue->type == 0) {
            // Internal node
            TParentsQueue *iter = parentsQueue;
            while (iter->next != NULL) {
                iter = iter->next;
            }
            iter->next = malloc(sizeof(TParentsQueue));
            iter = iter->next;
            iter->elem = parentsQueue->elem->kids[i];
            iter->next = NULL;
        } else {
            // Leaf node
            parentsQueue->elem->kids[i]->data.red = queue->data.red;
            parentsQueue->elem->kids[i]->data.green = queue->data.green;
            parentsQueue->elem->kids[i]->data.blue = queue->data.blue;
        }

        // Move to the next node in the queue
        if (queue->next == NULL) {
            return;
        }
        queue = queue->next;
    }

    // Move to the next parent in the queue
    parentsQueue = parentsQueue->next;
    buildTheQuadTreeFromQueue(parentsQueue, queue);
}