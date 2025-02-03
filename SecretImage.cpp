#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage& image) {

    width = image.get_width();
    height = image.get_height();

    // Calculate the size for the upper and lower triangular arrays
    int sizeOfUpper = (width * (width + 1)) / 2;
    int sizeOfLower = (width * (width - 1)) / 2;

    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    upper_triangular = new int[sizeOfUpper];
    lower_triangular = new int[sizeOfLower];

    // 2. Fill both matrices with the pixels from the GrayscaleImage.
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixelValue = image.get_pixel(i, j);

            if (i <= j) {
                //upper triangular matrix
                int indexOfUpper = (i * width - (i * (i - 1)) / 2) + (j - i);
                upper_triangular[indexOfUpper] = pixelValue;            
            } else {
                //lower triangular matrix
                int indexOfLower = (i * (i - 1)) / 2 + j;
                lower_triangular[indexOfLower] = pixelValue;            
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int * upper, int * lower) {
    width = w;
    height = h;
    
    upper_triangular = upper;
    lower_triangular = lower;

}

// Destructor: free the arrays
SecretImage::~SecretImage() {
    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    //fill the image with pixel values from upper and lower triangular matrices
   for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i <= j) {
                //get value from the upper triangular matrix (i <= j)
                int upperIndex = (i * width - (i * (i - 1)) / 2) + (j - i);
                image.set_pixel(i, j, upper_triangular[upperIndex]);
            } else {
                //get value from the lower triangular matrix (i > j)
                int lowerIndex = (i * (i - 1)) / 2 + j;
                image.set_pixel(i, j, lower_triangular[lowerIndex]);
            }
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage& image) {

    //recalculate the triangular arrays from the modified image
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int pixel_value = image.get_pixel(i, j);

            if (i <= j) {
                //save pixel value to the upper triangular matrix
                int index = (i * width - (i * (i - 1)) / 2) + (j - i);
                upper_triangular[index] = pixel_value;
            } else {
                //save pixel value to the lower triangular matrix
                int index = (i * (i - 1)) / 2 + j;
                lower_triangular[index] = pixel_value;
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string& filename) {

    std::ofstream file(filename);

    // Write width and height on the first line, separated by a single space.
    file << width << " " << height << "\n";

    // Write the upper_triangular array to the second line.
    int upper_size = (width * (width + 1)) / 2;
    for (int i = 0; i < upper_size; ++i) {
        file << upper_triangular[i] << " ";
    }
    file << "\n";

    // Write the lower_triangular array to the third line in a similar manner
    // as the second line.
    int lower_size = (width * (width - 1)) / 2;
    for (int i = 0; i < lower_size; ++i) {
        file << lower_triangular[i] << " ";
    }
    file.close();
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string& filename) {

    // Open the file and read width and height from the first line, separated by a space.
    std::ifstream file(filename);

    int w, h;
    file >> w >> h;

    // Calculate the sizes of the upper and lower triangular arrays.
    int upper_size = (w * (w + 1)) / 2;
    int lower_size = (w * (w - 1)) / 2;

    // Allocate memory for both arrays.
    int* upper = new int[upper_size];
    int* lower = new int[lower_size];

    // Read the upper_triangular array from the second line, space-separated.
    for (int i = 0; i < upper_size; ++i) {
        file >> upper[i];
    }

    // Read the lower_triangular array from the third line, space-separated.
    for (int i = 0; i < lower_size; ++i) {
        file >> lower[i];
    }

    // Close the file and return a SecretImage object initialized with the
    //width, height, and triangular arrays.
    file.close();

    SecretImage secret_image(w, h, upper, lower);
    return secret_image;
}

// Returns a pointer to the upper triangular part of the secret image.
int * SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int * SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
