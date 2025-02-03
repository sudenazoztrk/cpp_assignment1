#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>
#include <iostream>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage& image, int kernelSize) {
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.
    // 3. Update each pixel with the computed mean.

    int width = image.get_width();
    int height = image.get_height();
    int halfKernel = kernelSize / 2;

    // Orijinal görüntünün bir kopyasını oluştur.
    GrayscaleImage originalImage = image;

    // Yeni görüntü için bir matris oluştur
    int** newData = new int*[height];
    for (int i = 0; i < height; ++i) {
        newData[i] = new int[width];
    }

    // Her piksel için işlemi gerçekleştir.
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int sum = 0;
            int count = 0;

            // Kernel üzerindeki piksellerin ortalamasını hesapla.
            for (int ki = -halfKernel; ki <= halfKernel; ++ki) {
                for (int kj = -halfKernel; kj <= halfKernel; ++kj) {
                    int ni = i + ki;
                    int nj = j + kj;

                     // Kenarın dışına taşan durumları kontrol et ve dışa taşma durumunda 0 (siyah) ekle.
                    if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                        sum += originalImage.get_pixel(ni, nj);
                    } else {
                        sum += 0;  // Kernel taşarsa, siyah (0) pixel eklenir.
                    }
                    ++count;
                }
            }

            newData[i][j] = sum / count;

        }
    }

    // Yeni pikselleri orijinal görüntüye ata.
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image.set_pixel(i, j, newData[i][j]);
        }
    }

    // Belleği serbest bırak.
    for (int i = 0; i < height; ++i) {
        delete[] newData[i];
    }
    delete[] newData;
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage& image, int kernelSize, double sigma) {
    // TODO: Your code goes here.
    // 1. Create a Gaussian kernel based on the given sigma value.
    // 2. Normalize the kernel to ensure it sums to 1.
    // 3. For each pixel, compute the weighted sum using the kernel.
    // 4. Update the pixel values with the smoothed results.

    int width = image.get_width();
    int height = image.get_height();
    int halfKernel = kernelSize / 2;

    // Gaussian kernel oluştur
    std::vector<std::vector<double>> kernel(kernelSize, std::vector<double>(kernelSize));
    double sum = 0.0;

    for (int i = -halfKernel; i <= halfKernel; ++i) {
        for (int j = -halfKernel; j <= halfKernel; ++j) {
            double value = (1.0 / (2.0 * M_PI * sigma * sigma)) * exp(-(i * i + j * j) / (2.0 * sigma * sigma));
            kernel[i + halfKernel][j + halfKernel] = value;
            sum += value;
        }
    }


    // Kernel normalizasyonu
    for (int i = 0; i < kernelSize; ++i) {
        for (int j = 0; j < kernelSize; ++j) {
            kernel[i][j] /= sum;
        }
    }


    // Orijinal görüntünün bir kopyasını oluştur
    GrayscaleImage originalImage = image;
    std::vector<std::vector<int>> newData(height, std::vector<int>(width, 0));  // Yeni görüntü için vektör


        // Her piksel için Gaussian smoothing işlemi
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            double weightedSum = 0.0;

            for (int ki = -halfKernel; ki <= halfKernel; ++ki) {
                for (int kj = -halfKernel; kj <= halfKernel; ++kj) {
                    int ni = i + ki;
                    int nj = j + kj;

                    if (ni >= 0 && ni < height && nj >= 0 && nj < width) {
                        weightedSum += originalImage.get_pixel(ni, nj) * kernel[ki + halfKernel][kj + halfKernel];
                    }
                }
            }

            newData[i][j] = static_cast<int>(std::floor(weightedSum));
        }
    }

    // Yeni pikselleri ata
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            image.set_pixel(i, j, newData[i][j]);
        }
    }
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage& image, int kernelSize, double amount) {
     int width = image.get_width();
    int height = image.get_height();

    // Step 1: Apply Gaussian Smoothing to create a blurred version of the original image
    GrayscaleImage blurredImage = image;
    apply_gaussian_smoothing(blurredImage, kernelSize, 1.0);

    // Step 2: Unsharp masking formula
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            int originalPixel = image.get_pixel(i, j);
            int blurredPixel = blurredImage.get_pixel(i, j);
            int sharpenedPixel = originalPixel + (amount * (originalPixel - blurredPixel));

            // Clamp values using std::clamp to ensure they are within the valid range [0-255]
            if (sharpenedPixel > 255) {
                sharpenedPixel = 255;
            } else if (sharpenedPixel < 0) {
                sharpenedPixel = 0;
            }

            image.set_pixel(i, j, sharpenedPixel);
        }
    }
}

