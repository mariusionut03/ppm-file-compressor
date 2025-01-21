#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "dataTypes.h"

// Function to read a PPM file and return the image matrix
TPixel **readThePPMFile(char *fileName, unsigned int *fileSize);

// Function to compress an image matrix into a QuadTree
void compressToQuadTree(TQuadTree *node, int factor, int size, TPixel **mat);

// Function to free the memory allocated for a QuadTree
void freeQuadTree(TQuadTree* node);

// Function to count the number of leaves in a QuadTree
int numberOfLeavesQuadTree(TQuadTree* node);

// Function to calculate the height of a QuadTree
int heightQuadTree(TQuadTree* node);

// Function to write a QuadTree to a file
void writeQuadTree(TQuadTree* root, FILE* fp, int level);

// Function to write a QuadTree level by level to a file
void writeLevelByLevel(TQuadTree *root, FILE *fp);

// Helper function to write a specific level of a QuadTree to a file
void writeLevelByLevelHelper(TQuadTree *root, int level, FILE *fp);

#endif // COMPRESS_H