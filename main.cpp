#include <iostream>
#include <fftw3.h>
//#define cimg_use_png
#define cimg_display 0
#include "CImg.h"

using namespace cimg_library;

CImg<float> applyMedianFilter(const CImg<float>& img, int filter_size) {
    CImg<float> result = img;
    int rows = img.height();
    int cols = img.width();
    int half_size = filter_size / 2;

    std::vector<float> window{};
    for (int i = half_size; i < rows - half_size; ++i) {
        for (int j = half_size; j < cols - half_size; ++j) {
            for (int k = -half_size; k <= half_size; ++k) {
                for (int l = -half_size; l <= half_size; ++l) {
                    window.push_back(img(j + l, i + k));
                }
            }
            std::sort(window.begin(), window.end());
            result(j, i) = window[window.size() / 2];
            window.clear();
        }
    }
    return result;
}

// Function to apply a sharpening filter to an image
CImg<unsigned char> sharpenImage(const CImg<unsigned char>& image) {
    // Define a sharpening kernel (Laplacian kernel)
    const int kernel[3][3] = {
            {0, -1, 0},
            {-1, 5, -1},
            {0, -1, 0}
    };

    int width = image.width();
    int height = image.height();
    int channels = image.spectrum();

    CImg<unsigned char> result(width, height, 1, channels);

    // Apply convolution with the sharpening kernel
    for (int c = 0; c < channels; ++c) {
        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                int sum = 0;
                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        sum += image(x + kx, y + ky, 0, c) * kernel[ky + 1][kx + 1];
                    }
                }
                result(x, y, 0, c) = std::clamp(sum, 0, 255);
            }
        }
    }

    return result;
}

// Function to apply convolution to an image
CImg<unsigned char> applyConvolution(const CImg<unsigned char>& image, const std::vector<std::vector<int>>& kernel) {
    int rows = image.height();
    int cols = image.width();
    int kernelSize = kernel.size();
    int kCenter = kernelSize / 2;

    CImg<unsigned char> result(image.width(), image.height(), 1, image.spectrum(), 0);

    for (int y = kCenter; y < rows - kCenter; y++) {
        for (int x = kCenter; x < cols - kCenter; x++) {
            for (int c = 0; c < image.spectrum(); c++) {
                int sum = 0;
                for (int ky = 0; ky < kernelSize; ky++) {
                    for (int kx = 0; kx < kernelSize; kx++) {
                        int pixelVal = image(x + kx - kCenter, y + ky - kCenter, 0, c);
                        sum += kernel[ky][kx] * pixelVal;
                    }
                }
                result(x, y, 0, c) = std::clamp(sum, 0, 255);
            }
        }
    }
    return result;
}

// Function to normalize the image
CImg<unsigned char> normalizeImage(const CImg<unsigned char>& image) {
    CImg<unsigned char> result(image.width(), image.height(), 1, image.spectrum(), 0);
    for (int c = 0; c < image.spectrum(); c++) {
        float minVal = image.get_channel(c).min();
        float maxVal = image.get_channel(c).max();
        for (int y = 0; y < image.height(); y++) {
            for (int x = 0; x < image.width(); x++) {
                result(x, y, 0, c) = static_cast<unsigned char>(255 * (image(x, y, 0, c) - minVal) / (maxVal - minVal));
            }
        }
    }
    return result;
}

int main() {
    // Load the image
    const char* image_path = "input.png";
    CImg<unsigned char> image(image_path);
    int rows = image.height();
    int cols = image.width();
    //image = image.get_blur_median(3); // Apply median filter with a window size of 3

    // Split the image into its color channels
    CImg<float> red = image.get_channel(0);
    CImg<float> green = image.get_channel(1);
    CImg<float> blue = image.get_channel(2);

    // Apply median filter to each color channel
    int filter_size = 4; // Change the filter size as needed
    red = applyMedianFilter(red, filter_size);
    green = applyMedianFilter(green, filter_size);
    blue = applyMedianFilter(blue, filter_size);


    // Define a sharpening kernel (Laplacian kernel)
    std::vector<std::vector<int>> sharpenKernel = {
            {0, -1, 0},
            {-1, 5, -1},
            {0, -1, 0}
    };

    CImg<float> result_image(cols, rows, 1, 3);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result_image(j, i, 0) = red(j,i);
            result_image(j, i, 1) = green(j,i);
            result_image(j, i, 2) = blue(j,i);
        }
    }
    // Apply the convolution with the sharpening kernel
    auto sharpened_image = sharpenImage(result_image);
    sharpened_image.save("output.png");
    std::cout << "Image saved as 'output.png'" << std::endl;
    return 0;
}
