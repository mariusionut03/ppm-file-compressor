#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "dataTypes.h"

// Function to create a PPM file from a QuadTree
void createPPMfile(TQuadTree *node, TPixel **mat, int row_start, int row_end, int col_start, int col_end);

// Function to build a QuadTree from a queue
void buildTheQuadTreeFromQueue(TParentsQueue *parentsQueue, TQueue *queue);

#endif // DECOMPRESS_H