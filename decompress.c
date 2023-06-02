/* BUZOI Marius-Ionut */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dataTypes.h"
#include "decompress.h"

/* Build a pixel matrix based on a quadtree */
void createPPMfile(TQuadTree *node, TPixel **mat, int rowStart, int rowEnd, int colStart, int colEnd) {
    if(node->kids[0] == NULL && node->kids[1] == NULL && node->kids[2] == NULL && node->kids[3] == NULL) {
        // Leaf node, we fill the block with the values
        for (int i = rowStart; i < rowEnd; i++) {
            for (int j = colStart; j < colEnd; j++) {
                mat[i][j].red = node->data.red;
                mat[i][j].green = node->data.green;
                mat[i][j].blue = node->data.blue;
            }
        }
    } else {
        // Intern node, divide again
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
    for (int i = 0; i < 4; i++) {
        parentsQueue->elem->kids[i] = malloc(sizeof(TQuadTree));
        parentsQueue->elem->kids[i]->data.red = parentsQueue->elem->kids[i]->data.green = parentsQueue->elem->kids[i]->data.blue = 0;
        for(int j = 0; j < 4; j++) {
            parentsQueue->elem->kids[i]->kids[j] = NULL;
        }
    }
    for (int i = 0; i < 4; i++) {    
        if (queue->type == 0) {
            // Intern node
            TParentsQueue *iter = parentsQueue;
            while(iter->next != NULL) {
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
        if (queue->next == NULL) {
            return;
        }
        queue = queue->next;
    }
    parentsQueue = parentsQueue->next;
    buildTheQuadTreeFromQueue(parentsQueue, queue);
}