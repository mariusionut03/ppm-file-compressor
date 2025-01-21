# PPM File Compressor

## Overview
This program in C uses quad trees to compress PPM image files. It works only for images with the same width and height.

## Components
- `src/compress.c(.h)` - Implements functions to compress an image matrix into a QuadTree.
- `src/decompress.c(.h)` - Implements functions to decompress a QuadTree back into an image matrix.
- `src/dataTypes.h` - Defines the data structures used for pixels, QuadTrees, and queues.
- `main.c` - Contains the main function to handle command-line arguments and perform compression or decompression.

## Features

- **Compression**: 
  - Reads the PPM file and saves the pixel values into a matrix defined with the `TPixel` type.
  - Uses a recursive function to sum up the values in each channel of all the pixels, calculates the mean for each channel, and determines a score.
  - If the score is less than or equal to the factor, it saves the mean values of red, green, and blue for all the pixels in the current node.
  - If not, it divides the block into 4 smaller blocks, allocates memory for 4 new nodes, and recalls the function for each block.
  - At the end of the recursive function, a QuadTree is built. Inside nodes are blocks that were divided, and leaf nodes hold colors for undivided blocks.
  - Writes the QuadTree level by level to a binary file, saving the size of the PPM file before all of this.

- **Decompression**:
  - Rebuilds the QuadTree based on the values in the file and creates a square matrix of pixels based on the size found at the start of the binary file.
  - Uses a recursive function to divide the matrix into 4 smaller blocks while iterating through the QuadTree. When a leaf node is found, it fills the block with the values inside the node.
  - Writes the header of a PPM file into the file passed as a parameter when running the program, then iterates through the matrix and writes the values of each pixel in the file.

## Usage

### Compress
1. Open a Bash terminal and go to the code location.
2. Use `make` to create the executable.
3. Locate your PPM file that meets the requirements (height == width) and type:
   ```sh
   ./cppm -c [Factor] [PPM_FILE_PATH] [OUT_FILE_PATH]
   ```
    - **Factor**: The higher the value, the worse the quality. Use 0 to compress the image with the default quality (not recommended for big images or real-life photos).
    - **PPM_FILE_PATH**: The path to the PPM file you want to compress.
    - **OUT_FILE_PATH**: The path where you want to save the binary file with the compressed image.

    Example:
    ```sh
    ./cppm -c 100 image.ppm image.out
    ```
### Decompress
1. Use:
    ```sh
    ./cppm -d [COMPRESSED_FILE_PATH] [DECOMPRESSED_PPM_FILE]
    ```
    - **COMPRESSED_FILE_PATH**: The path to the compressed file.
    - **DECOMPRESSED_PPM_FILE**: The path where you want to save the decompressed PPM file.
    
    Example:
    ```sh
    ./cppm -d image.out image.ppm
    ```

    Use the file at `images/test.ppm` for tests.

# Examples

<img src="./images/test.png" width="300">
<img src="./images/test20.png" width="300">
<img src="./images/test50.png" width="300">
<img src="./images/test100.png" width="300">
<img src="./images/test200.png" width="300">
<img src="./images/test500.png" width="300">
<img src="./images/test1000.png" width="300">