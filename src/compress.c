#include "compress.h"

/* This function gets the path to a PPM file and returns a matrix with its pixels */
TPixel **readThePPMFile(char *fileName, unsigned int *fileSize) {
    FILE *fin = fopen(fileName, "r");
    char *fileType = malloc(sizeof(char) * 3);
    fread(fileType, sizeof(char), 2, fin);
    int fileValueMaxColor;
    fscanf(fin, "%d%d%d", fileSize, fileSize, &fileValueMaxColor);
    char *buffer = malloc(sizeof(char) * 2);
    fread(buffer, sizeof(char), 1, fin);
    TPixel **mat = malloc(sizeof(TPixel*) * (*fileSize));
    for (int i = 0; i < *fileSize; i++) {
        mat[i] = malloc(sizeof(TPixel) * (*fileSize));
        for (int j = 0; j < *fileSize; j++) {
            fread(&mat[i][j], sizeof(TPixel), 1, fin);
        }
    }
    free(fileType);
    free(buffer);
    return mat;
}

/* Gets a pixel matrix and returns a quadtree built by the rules presented in README */
void compressToQuadTree(TQuadTree *node, int factor, int size, TPixel **mat) {
    unsigned long long int red = 0, green = 0, blue = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            red += mat[i][j].red;
            green += mat[i][j].green;
            blue += mat[i][j].blue;
        }
    }
    red = floor(red / (size * size));
    green = floor(green / (size * size));
    blue = floor(blue / (size * size));
    unsigned long long int mean = 0;
    unsigned long long int helper = 0;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            helper = helper + (red - mat[i][j].red) * (red - mat[i][j].red);
            helper = helper + (green - mat[i][j].green) * (green - mat[i][j].green);
            helper = helper + (blue - mat[i][j].blue) * (blue - mat[i][j].blue);
            mean += helper;
            helper = 0;
        }
    }
    mean = floor(mean / (3 * (size * size)));
    if (mean <= factor) {
        // The block meets the factor rule, so we stop the division
        node->data.red = red;
        node->data.green = green;
        node->data.blue = blue;
        for (int i = 0; i < size; i++) {
            free(mat[i]);
        }
        free(mat);
    } else {
        // We divide the block
        for (int i = 0; i < 4; i++) {
            node->kids[i] = malloc(sizeof(TQuadTree));
            node->kids[i]->data.red = node->kids[i]->data.green = node->kids[i]->data.blue = 0;
            for (int j = 0; j < 4; j++) {
                node->kids[i]->kids[j] = NULL;
            }
        }

        TPixel **tl = malloc(sizeof(TPixel*) * (size / 2));
        TPixel **tr = malloc(sizeof(TPixel*) * (size / 2));
        TPixel **br = malloc(sizeof(TPixel*) * (size / 2));
        TPixel **bl = malloc(sizeof(TPixel*) * (size / 2));
        for (int i = 0; i < (size / 2); i++) {
            tl[i] = malloc(sizeof(TPixel) * (size / 2));
            tr[i] = malloc(sizeof(TPixel) * (size / 2));
            br[i] = malloc(sizeof(TPixel) * (size / 2));
            bl[i] = malloc(sizeof(TPixel) * (size / 2));
        }

        for (int i = 0; i < (size / 2); i++) {
            for (int j = 0; j < (size / 2); j++) {
                tl[i][j].red = mat[i][j].red;
                tl[i][j].green = mat[i][j].green;
                tl[i][j].blue = mat[i][j].blue;

                tr[i][j].red = mat[i][j + (size / 2)].red;
                tr[i][j].green = mat[i][j + (size / 2)].green;
                tr[i][j].blue = mat[i][j + (size / 2)].blue;

                br[i][j].red = mat[i + (size / 2)][j + (size / 2)].red;
                br[i][j].green = mat[i + (size / 2)][j + (size / 2)].green;
                br[i][j].blue = mat[i + (size / 2)][j + (size / 2)].blue;

                bl[i][j].red = mat[i + (size / 2)][j].red;
                bl[i][j].green = mat[i + (size / 2)][j].green;
                bl[i][j].blue = mat[i + (size / 2)][j].blue;
            }
        }
        for (int i = 0; i < size; i++) {
            free(mat[i]);
        }
        free(mat);
        mat = NULL;
        compressToQuadTree(node->kids[0], factor, size / 2, tl);
        compressToQuadTree(node->kids[1], factor, size / 2, tr);
        compressToQuadTree(node->kids[2], factor, size / 2, br);
        compressToQuadTree(node->kids[3], factor, size / 2, bl);
    }
}

/* Function to return the height of a quadtree */
int heightQuadTree(TQuadTree* node) {
    if (node == NULL) {
        return 0;
    }

    int heights[4] = {
        heightQuadTree(node->kids[0]),
        heightQuadTree(node->kids[1]),
        heightQuadTree(node->kids[2]),
        heightQuadTree(node->kids[3])
    };

    int max_height = heights[0];
    for (int i = 1; i < 4; i++) {
        if (heights[i] > max_height) {
            max_height = heights[i];
        }
    }
    return 1 + max_height;
}

/* Return the number of leaves in a quadtree */
int numberOfLeavesQuadTree(TQuadTree* node) {
    if (node == NULL) {
        return 0;
    }

    if (node->kids[0] == NULL && node->kids[1] == NULL && node->kids[2] == NULL && node->kids[3] == NULL) {
        return 1;
    }

    int leaf_count = 0;
    leaf_count += numberOfLeavesQuadTree(node->kids[0]);
    leaf_count += numberOfLeavesQuadTree(node->kids[1]);
    leaf_count += numberOfLeavesQuadTree(node->kids[2]);
    leaf_count += numberOfLeavesQuadTree(node->kids[3]);

    return leaf_count;
}

/* Free memory of a quadtree */
void freeQuadTree(TQuadTree* node) {
    if (node == NULL) {
        return;
    }

    freeQuadTree(node->kids[0]);
    freeQuadTree(node->kids[1]);
    freeQuadTree(node->kids[2]);
    freeQuadTree(node->kids[3]);

    free(node);
}

/* Helper function to write a quadtree level by level to a file */
void writeLevelByLevelHelper(TQuadTree *root, int level, FILE *fp) {
    if (root == NULL) {
        return;
    } else if (level == 1) {
        if (root->kids[0] == NULL && root->kids[1] == NULL && root->kids[2] == NULL && root->kids[3] == NULL) {
            unsigned char isLeaf = 1;
            fwrite(&isLeaf, sizeof(unsigned char), 1, fp);
            fwrite(&(root->data.red), sizeof(unsigned char), 1, fp);
            fwrite(&(root->data.green), sizeof(unsigned char), 1, fp);
            fwrite(&(root->data.blue), sizeof(unsigned char), 1, fp);
        } else {
            unsigned char isLeaf = 0;
            fwrite(&isLeaf, sizeof(unsigned char), 1, fp);
        }
    } else {
        for (int i = 0; i < 4; i++) {
            writeLevelByLevelHelper(root->kids[i], level - 1, fp);
        }
    }
}

/* Write a quadtree level by level to a file */
void writeLevelByLevel(TQuadTree *root, FILE *fp) {
    int height = heightQuadTree(root);
    for (int i = 1; i <= height; i++) {
        writeLevelByLevelHelper(root, i, fp);
    }
}