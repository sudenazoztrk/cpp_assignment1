#include "GrayscaleImage.h"
#include <iostream>
#include <cstring>  // For memcpy
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <stdexcept>


// Constructor: load from a file
GrayscaleImage::GrayscaleImage(const char* filename) {

    // Image loading code using stbi
    int channels;
    unsigned char* image = stbi_load(filename, &width, &height, &channels, STBI_grey);

    //check if the image was loaded successfully
    if (image == nullptr) {
        std::cerr << "Error: Could not load image " << filename << std::endl;
        exit(1);
    }

    //dynamically allocate memory for a 2D matrix to store the image data
    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
    }

    //copy pixel values from the loaded image to the data matrix
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            data[i][j] = image[i * width + j];
        }
    }

    // Free the dynamically allocated memory of stbi image
    stbi_image_free(image);
    
}

// Constructor: initialize from a pre-existing data matrix
GrayscaleImage::GrayscaleImage(int** inputData, int h, int w) {

    // dynamically allocate memory for the data matrix
    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
    }

    //copy the input data to the matrix
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            data[i][j] = inputData[i][j];
        }
    }

}

// Constructor to create a blank image of given width and height
GrayscaleImage::GrayscaleImage(int w, int h) : width(w), height(h) {

    //dynamically allocate memory for the data matrix
    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
        for (int j = 0; j < width; ++j) {
            data[i][j] = 0; //initialize all pixel values to zero(black)
        }
    }
}

// Copy constructor
GrayscaleImage::GrayscaleImage(const GrayscaleImage& other) {

    //copy the dimention
    width = other.width;
    height = other.height;

   //dynamically allocate memory for the data matrix
    data = new int*[height];
    for (int i = 0; i < height; ++i) {
        data[i] = new int[width];
    }

    //copy pixel values from the other image to the new one
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            data[i][j] = other.data[i][j];
        }
    }
}

// Destructor
GrayscaleImage::~GrayscaleImage() {

    for (int i = 0; i < height; ++i) {
        delete[] data[i]; // delete each row
    }
    delete[] data; // delete the array of rows
    
}

// Equality operator
bool GrayscaleImage::operator==(const GrayscaleImage& other) const {

    //check if dimensions match
    if (width != other.width || height != other.height) {
        return false;
    }

    //check if all pixel values are the same
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (data[i][j] != other.data[i][j]) {
                return false;
            }
        }
    }

    return true;
}

// Addition operator
GrayscaleImage GrayscaleImage::operator+(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    //add pixel values of both images
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int total = data[i][j] + other.data[i][j];
            if (total > 255) { //pixel values have to be in [0,255] range
                result.data[i][j] = 255;
            } else {
            result.data[i][j] = total;
            }
        }
    }

    return result;
}


// Subtraction operator
GrayscaleImage GrayscaleImage::operator-(const GrayscaleImage& other) const {
    // Create a new image for the result
    GrayscaleImage result(width, height);
    
    //substract pixels of two images
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int difference = data[i][j] - other.data[i][j];
            if (difference < 0) { //pixel values have to be in [0,255] range
                result.data[i][j] = 0;
            } else {
            result.data[i][j] = difference;
            }
        }
    }

    return result;
}

// Get a specific pixel value
int GrayscaleImage::get_pixel(int row, int col) const {
    return data[row][col];
}

// Set a specific pixel value
void GrayscaleImage::set_pixel(int row, int col, int value) {
    data[row][col] = value;
}

// Function to save the image to a PNG file
void GrayscaleImage::save_to_file(const char* filename) const {
    // Create a buffer to hold the image data in the format stb_image_write expects
    unsigned char* imageBuffer = new unsigned char[width * height];

    // Fill the buffer with pixel data (convert int to unsigned char)
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            imageBuffer[i * width + j] = static_cast<unsigned char>(data[i][j]);
        }
    }

    // Write the buffer to a PNG file
    if (!stbi_write_png(filename, width, height, 1, imageBuffer, width)) {
        std::cerr << "Error: Could not save image to file " << filename << std::endl;
    }

    // Clean up the allocated buffer
    delete[] imageBuffer;
}

