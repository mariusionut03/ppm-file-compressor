/* BUZOI Marius-Ionut */

TPixel **readThePPMFile(char *fileName, unsigned int *fileSize);
void compressToQuadTree(TQuadTree *node, int factor, int size, TPixel **mat);
void freeQuadTree(TQuadTree* node);
int numberOfLeavesQuadTree(TQuadTree* node);
int heightQuadTree(TQuadTree* node);
void writeQuadTree(TQuadTree* root, FILE* fp, int level);
void writeLevelByLevel(TQuadTree *root, FILE *fp);
void writeLevelByLevelHelper(TQuadTree *root, int level, FILE *fp);