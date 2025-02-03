#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage& secret_image, int message_length) {
    std::vector<int> LSB_array;

    // Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage image = secret_image.reconstruct();

    // Calculate the image dimensions.
    int width = image.get_width();
    int height = image.get_height();

    // Determine the total bits required based on message length.
    int totalBits = message_length * 7;

    // Ensure the image has enough pixels; if not, throw an error.
    if (width * height < totalBits) {
        throw std::runtime_error("Image is too small to contain the secret message.");
    }

    int totalPixels = width * height;
    int startPixel = totalPixels - totalBits;

    int lsb_index = 0;  
    for (int i = startPixel; i < totalPixels; ++i) {
        int r = i / width;
        int c = i % width;

        int pixelValue = image.get_pixel(r, c);

        LSB_array.push_back(pixelValue & 1); 
        
        lsb_index++;
    }

    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int>& LSB_array) {
    std::string message;

    // Verify that the LSB array size is a multiple of 7, else throw an error.
    if (LSB_array.size() % 7 != 0) {
        throw std::runtime_error("LSB array size is not a multiple of 7.");
    }

    // Convert each group of 7 bits into an ASCII character.
    for (size_t i = 0; i < LSB_array.size(); i += 7) {
        int charValue = 0;
        for (int j = 0; j < 7; ++j) {
            charValue = (charValue << 1) | LSB_array[i + j];
        }
        message += static_cast<char>(charValue);
    }

    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string& message) {
    std::vector<int> LSB_array;

    // Convert each character of the message into a 7-bit binary representation.
    for (char chr : message) {
        std::bitset<7> bits(chr);

        // Collect the bits into the LSB array.
        for (int i = 6; i >= 0; --i) {
            LSB_array.push_back(bits[i]);
        }
    }
    // Return the array of bits.

    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage& image, const std::vector<int>& LSB_array) {

    // Check if the image has enough pixels to store the LSB array
    int totalPixels = image.get_width() * image.get_height();
    

    // Calculate the starting pixel index, so the last LSB ends up in the last pixel of the image.
    int startPixel = totalPixels - LSB_array.size();

    // Embed the LSB array into the image
    int lsbIndex = 0;  // Index for iterating through LSB_array
    for (int i = startPixel; i < totalPixels; ++i) {
        int r = i / image.get_width();
        int c = i % image.get_width();

        // Get the current pixel value
        int pixelValue = image.get_pixel(r, c);

        // Clear the LSB (make it 0)
        pixelValue = pixelValue & ~1;

        // Set the new LSB from the array
        pixelValue = pixelValue | LSB_array[lsbIndex];

        // Update the pixel in the image
        image.set_pixel(r, c, pixelValue);

        lsbIndex++;
    }

    // Return a SecretImage object constructed from the modified GrayscaleImage
    SecretImage secretImage(image);
    return secretImage;
}
