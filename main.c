/* BUZOI Marius-Ionut */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "dataTypes.h"
#include "compress.h"
#include "decompress.h"

int main(int argc, char **argv) {
    unsigned int sizeOfImage = 0;
    if (!strcmp(argv[1], "-c")) {
        TPixel **mat = readThePPMFile(argv[3], &sizeOfImage);
        TQuadTree *quadTree = malloc(sizeof(TQuadTree));
        quadTree->data.red = quadTree->data.green = quadTree->data.blue = 0;
        for (int i = 0; i < 4; i++) {
            quadTree->kids[i] = NULL;
        }
        compressToQuadTree(quadTree, atoi(argv[2]), sizeOfImage, mat);
        FILE *fin = fopen(argv[4], "wb");
        fwrite(&sizeOfImage, sizeof(unsigned int), 1, fin);
        writeLevelByLevel(quadTree, fin);
        freeQuadTree(quadTree);
        fclose(fin);
    } else {
        TQuadTree *quadTree = malloc(sizeof(TQuadTree));
        quadTree->data.red = quadTree->data.green = quadTree->data.blue = 0;
        for (int i = 0; i < 4; i++) {
            quadTree->kids[i] = NULL;
        }
        quadTree->type = 0;
        TQueue *queue = malloc(sizeof(TQueue));
        queue->data.red = queue->data.green = queue->data.blue = 0;
        queue->next = NULL;
        queue->type = 0;
        TQueue *queueHead = queue;

        FILE *compFile = fopen(argv[2], "rb");
        fread(&sizeOfImage, sizeof(unsigned int), 1, compFile);
        printf("Image size: %d\n", sizeOfImage);
        unsigned char buffer = 0;
        fread(&buffer, sizeof(unsigned char), 1, compFile);
        if(buffer == 0) {
            queue->data.red = queue->data.green = queue->data.blue = 0;
            queue->type = 0;
        } else {
            fread(&(queue->data.red), sizeof(unsigned char), 1, compFile);
            fread(&(queue->data.green), sizeof(unsigned char), 1, compFile);
            fread(&(queue->data.blue), sizeof(unsigned char), 1, compFile);
            queue->type = 1;
        }

        TQueue *iter = queue;
        while(fread(&buffer, sizeof(unsigned char), 1, compFile) != 0) {
            if(buffer == 1) {
                iter->next = malloc(sizeof(TQueue));
                iter = iter->next;
                iter->data.red = iter->data.green = iter->data.blue = 0;
                iter->next = NULL;
                fread(&(iter->data.red), sizeof(unsigned char), 1, compFile);
                fread(&(iter->data.green), sizeof(unsigned char), 1, compFile);
                fread(&(iter->data.blue), sizeof(unsigned char), 1, compFile);
                iter->type = 1;
            } else {
                iter->next = malloc(sizeof(TQueue));
                iter = iter->next;
                iter->data.red = iter->data.green = iter->data.blue = 0;
                iter->type = 0;
                iter->next = NULL;
            }
        }

       TParentsQueue *parentsQueue = malloc(sizeof(TParentsQueue));
       parentsQueue->elem = NULL;
       parentsQueue->next = NULL;
       TParentsQueue *parentsQueueHead = parentsQueue;
        if (queue->type == 0) {
            parentsQueue->elem = quadTree;
            queue = queue->next;
            buildTheQuadTreeFromQueue(parentsQueue, queue);
        } else {
            quadTree->data.red = queue->data.red;
            quadTree->data.green = queue->data.green;
            quadTree->data.blue = queue->data.blue;
        }


        TPixel **rebuiltMat = calloc(sizeOfImage, sizeof(TPixel*));
        for (int i = 0; i < sizeOfImage; i++) {
            rebuiltMat[i] = calloc(sizeOfImage, sizeof(TPixel));
        }


        createPPMfile(quadTree, rebuiltMat, 0, sizeOfImage, 0, sizeOfImage);

        FILE *fin = fopen(argv[3], "wb");
        fprintf(fin, "%s\n%d %d\n%d\n", "P6", sizeOfImage, sizeOfImage, 255);
        for(int i = 0; i < sizeOfImage; i++) {
            for(int j = 0 ; j < sizeOfImage; j++) {
                fwrite(&(rebuiltMat[i][j].red), sizeof(unsigned char), 1, fin);
                fwrite(&(rebuiltMat[i][j].green), sizeof(unsigned char), 1, fin);
                fwrite(&(rebuiltMat[i][j].blue), sizeof(unsigned char), 1, fin);
            }
        }
        TQueue *temp = queueHead;
        while(temp != NULL) {
            TQueue *current = temp;
            temp = temp->next;
            free(current);
        }
        queueHead = NULL;
        TParentsQueue *tempParents = parentsQueueHead;
        while(tempParents != NULL) {
            TParentsQueue *currentParent = tempParents;
            tempParents = tempParents->next;
            free(currentParent);
        }
        parentsQueueHead = NULL;
        freeQuadTree(quadTree);
        for(int i = 0; i < sizeOfImage; i++) {
            free(rebuiltMat[i]);
        }
        free(rebuiltMat);
        fclose(fin);
        fclose(compFile);
    }
    return 0;
}
