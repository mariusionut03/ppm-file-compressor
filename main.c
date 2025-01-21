#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "src/dataTypes.h"
#include "src/compress.h"
#include "src/decompress.h"

int main(int argc, char **argv) {
    unsigned int sizeOfImage = 0;

    // Check if the operation is compression
    if (!strcmp(argv[1], "-c")) {
        // Read the PPM file and get the image matrix
        TPixel **mat = readThePPMFile(argv[3], &sizeOfImage);

        // Initialize the QuadTree
        TQuadTree *quadTree = malloc(sizeof(TQuadTree));
        quadTree->data.red = quadTree->data.green = quadTree->data.blue = 0;
        for (int i = 0; i < 4; i++) {
            quadTree->kids[i] = NULL;
        }

        // Compress the image to QuadTree
        compressToQuadTree(quadTree, atoi(argv[2]), sizeOfImage, mat);

        // Write the compressed data to file
        FILE *fin = fopen(argv[4], "wb");
        fwrite(&sizeOfImage, sizeof(unsigned int), 1, fin);
        writeLevelByLevel(quadTree, fin);

        // Free allocated memory
        freeQuadTree(quadTree);
        fclose(fin);
    } else {
        // Initialize the QuadTree
        TQuadTree *quadTree = malloc(sizeof(TQuadTree));
        quadTree->data.red = quadTree->data.green = quadTree->data.blue = 0;
        for (int i = 0; i < 4; i++) {
            quadTree->kids[i] = NULL;
        }
        quadTree->type = 0;

        // Initialize the Queue
        TQueue *queue = malloc(sizeof(TQueue));
        queue->data.red = queue->data.green = queue->data.blue = 0;
        queue->next = NULL;
        queue->type = 0;
        TQueue *queueHead = queue;

        // Read the compressed file
        FILE *compFile = fopen(argv[2], "rb");
        fread(&sizeOfImage, sizeof(unsigned int), 1, compFile);
        printf("Image size: %d\n", sizeOfImage);

        // Read the first node
        unsigned char buffer = 0;
        fread(&buffer, sizeof(unsigned char), 1, compFile);
        if (buffer == 0) {
            queue->data.red = queue->data.green = queue->data.blue = 0;
            queue->type = 0;
        } else {
            fread(&(queue->data.red), sizeof(unsigned char), 1, compFile);
            fread(&(queue->data.green), sizeof(unsigned char), 1, compFile);
            fread(&(queue->data.blue), sizeof(unsigned char), 1, compFile);
            queue->type = 1;
        }

        // Read the rest of the nodes
        TQueue *iter = queue;
        while (fread(&buffer, sizeof(unsigned char), 1, compFile) != 0) {
            iter->next = malloc(sizeof(TQueue));
            iter = iter->next;
            iter->data.red = iter->data.green = iter->data.blue = 0;
            iter->next = NULL;
            if (buffer == 1) {
                fread(&(iter->data.red), sizeof(unsigned char), 1, compFile);
                fread(&(iter->data.green), sizeof(unsigned char), 1, compFile);
                fread(&(iter->data.blue), sizeof(unsigned char), 1, compFile);
                iter->type = 1;
            } else {
                iter->type = 0;
            }
        }

        // Initialize the ParentsQueue
        TParentsQueue *parentsQueue = malloc(sizeof(TParentsQueue));
        parentsQueue->elem = NULL;
        parentsQueue->next = NULL;
        TParentsQueue *parentsQueueHead = parentsQueue;

        // Build the QuadTree from the queue
        if (queue->type == 0) {
            parentsQueue->elem = quadTree;
            queue = queue->next;
            buildTheQuadTreeFromQueue(parentsQueue, queue);
        } else {
            quadTree->data.red = queue->data.red;
            quadTree->data.green = queue->data.green;
            quadTree->data.blue = queue->data.blue;
        }

        // Allocate memory for the rebuilt image matrix
        TPixel **rebuiltMat = calloc(sizeOfImage, sizeof(TPixel *));
        for (int i = 0; i < sizeOfImage; i++) {
            rebuiltMat[i] = calloc(sizeOfImage, sizeof(TPixel));
        }

        // Create the PPM file from the QuadTree
        createPPMfile(quadTree, rebuiltMat, 0, sizeOfImage, 0, sizeOfImage);

        // Write the rebuilt image to file
        FILE *fin = fopen(argv[3], "wb");
        fprintf(fin, "%s\n%d %d\n%d\n", "P6", sizeOfImage, sizeOfImage, 255);
        for (int i = 0; i < sizeOfImage; i++) {
            for (int j = 0; j < sizeOfImage; j++) {
                fwrite(&(rebuiltMat[i][j].red), sizeof(unsigned char), 1, fin);
                fwrite(&(rebuiltMat[i][j].green), sizeof(unsigned char), 1, fin);
                fwrite(&(rebuiltMat[i][j].blue), sizeof(unsigned char), 1, fin);
            }
        }

        // Free allocated memory
        TQueue *temp = queueHead;
        while (temp != NULL) {
            TQueue *current = temp;
            temp = temp->next;
            free(current);
        }
        queueHead = NULL;

        TParentsQueue *tempParents = parentsQueueHead;
        while (tempParents != NULL) {
            TParentsQueue *currentParent = tempParents;
            tempParents = tempParents->next;
            free(currentParent);
        }
        parentsQueueHead = NULL;

        freeQuadTree(quadTree);
        for (int i = 0; i < sizeOfImage; i++) {
            free(rebuiltMat[i]);
        }
        free(rebuiltMat);
        fclose(fin);
        fclose(compFile);
    }
    return 0;
}