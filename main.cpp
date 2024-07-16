#include <iostream>
#include <vector>
#include <algorithm>
#include "gperftools/profiler.h"

#define cimg_use_fftw3
#define cimg_display 0
#include "CImg.h"

using namespace cimg_library;

// Function to apply a median filter to an image
CImg<float> applyMedianFilter(const CImg<float> &img, int filter_size) {
    return img.get_blur_median(filter_size);
}

// Function to apply a sharpening filter to an image
CImg<unsigned char> sharpenImage(const CImg<unsigned char> &image) {
    const int kernel[3][3] = {
            {0,  -1, 0},
            {-1, 5,  -1},
            {0,  -1, 0}
    };

    int width = image.width();
    int height = image.height();
    int channels = image.spectrum();

    CImg<unsigned char> result(width, height, 1, channels);

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

CImgList<float> fft(const CImg<float>& img) {
    return img.get_FFT();
}

CImg<float> ifft(const CImgList<float>& fft) {
    return fft.get_FFT(true)[0];
}


void applyHighPassFilter(CImgList<float>& fft, float cutoff) {
    int width = fft[0].width();
    int height = fft[0].height();
    int cx = width / 2;
    int cy = height / 2;

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float dist = std::sqrt(std::pow(x - cx, 2) + std::pow(y - cy, 2));
            if (dist < cutoff) {
                fft[0](x, y) = 0;
                fft[1](x, y) = 0;
            }
        }
    }
}

CImg<unsigned char> edgeDetection(const CImg<unsigned char>& image, float cutoff) {
    // Convert image to grayscale
    CImg<float> gray = image.get_RGBtoYCbCr().channel(0);

    // Perform FFT
    CImgList<float> fftResult = fft(gray);

    // Apply high-pass filter
    applyHighPassFilter(fftResult, cutoff);

    // Perform inverse FFT
    CImg<float> filteredImage = ifft(fftResult);

    // Normalize the result
    filteredImage.normalize(0, 255);

    return filteredImage;
}

int main(const int argc, const char **argv) {
    ProfilerStart("sample.prof");
    // Load the image
    const char *image_path = "input.png";
    CImg<unsigned char> image(image_path);
    int rows = image.height();
    int cols = image.width();

    // Split the image into its color channels
    CImg<float> red = image.get_channel(0);
    CImg<float> green = image.get_channel(1);
    CImg<float> blue = image.get_channel(2);

    // Apply median filter to each color channel
    int filter_size = 4; // Change the filter size as needed
    red = applyMedianFilter(red, filter_size);
    green = applyMedianFilter(green, filter_size);
    blue = applyMedianFilter(blue, filter_size);

    // Merge the color channels back together
    CImg<float> result_image(cols, rows, 1, 3);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            result_image(j, i, 0) = red(j, i);
            result_image(j, i, 1) = green(j, i);
            result_image(j, i, 2) = blue(j, i);
        }
    }

    // Apply the sharpening filter
    auto sharpened_image = sharpenImage(result_image);
    sharpened_image.save("sharpened_output.png");

    // Apply FFT-based edge detection
    // Perform edge detection
    float cutoff = 1.f;  // Adjust this value to control the cutoff frequency
    CImg<unsigned char> edge_image = edgeDetection(result_image, cutoff);

    ProfilerStop();
    // Save the edge-detected image
    edge_image.save("edge_output.png");
    std::cout << "Images saved as 'sharpened_output.png' and 'edge_output.png'" << std::endl;
    return 0;
}
