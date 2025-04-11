#include "metrics.hpp"
#include <cmath>
#include <iostream>
#include <array>

double computeVariance(const unsigned char* imageData, int x, int y, int width, int height, int imageWidth, int channels) {
    long long sumR = 0, sumG = 0, sumB = 0;
    int N = width * height;
    
    for (int j = y; j < y + height; ++j) {
        for (int i = x; i < x + width; ++i) {
            int idx = (j * imageWidth + i) * channels;
            sumR += imageData[idx];
            sumG += imageData[idx + 1];
            sumB += imageData[idx + 2];
        }
    }

    double meanR = static_cast<double>(sumR) / N;
    double meanG = static_cast<double>(sumG) / N;
    double meanB = static_cast<double>(sumB) / N;

    double varR = 0.0, varG = 0.0, varB = 0.0;

    for (int j = y; j < y + height; ++j) {
        for (int i = x; i < x + width; ++i) {
            int idx = (j * imageWidth + i) * channels;
            varR += (imageData[idx] - meanR) * (imageData[idx] - meanR);
            varG += (imageData[idx + 1] - meanG) * (imageData[idx + 1] - meanG);
            varB += (imageData[idx + 2] - meanB) * (imageData[idx + 2] - meanB);
        }
    }

    varR /= N;
    varG /= N;
    varB /= N;

    double variance = (varR + varG + varB) / 3.0;
    return variance;
}

double computeMAD(const unsigned char* imageData, int x, int y, int width, int height, int imageWidth, int channels) {
    long long sumR = 0, sumG = 0, sumB = 0;
    int N = width * height;
    
    for (int j = y; j < y + height; ++j) {
        for (int i = x; i < x + width; ++i) {
            int idx = (j * imageWidth + i) * channels;
            sumR += imageData[idx];
            sumG += imageData[idx + 1];
            sumB += imageData[idx + 2];
        }
    }

    double meanR = static_cast<double>(sumR) / N;
    double meanG = static_cast<double>(sumG) / N;
    double meanB = static_cast<double>(sumB) / N;

    double madR = 0.0, madG = 0.0, madB = 0.0;

    for (int j = y; j < y + height; ++j) {
        for (int i = x; i < x + width; ++i) {
            int idx = (j * imageWidth + i) * channels;
            madR += std::abs(imageData[idx] - meanR);
            madG += std::abs(imageData[idx + 1] - meanG);
            madB += std::abs(imageData[idx + 2] - meanB);
        }
    }

    madR /= N;
    madG /= N;
    madB /= N;

    double mad = (madR + madG + madB) / 3.0;
    return mad;
}

double computeMaxDiff(const unsigned char* imageData, int x, int y, int width, int height, int imageWidth, int channels) {
    int minR = 255, minG = 255, minB = 255;
    int maxR = 0, maxG = 0, maxB = 0;

    for (int j = y; j < y + height; j++) {
        for (int i = x; i < x + width; i++) {
            int idx = (j * imageWidth + i) * channels;
            unsigned char r = imageData[idx];
            unsigned char g = imageData[idx + 1];
            unsigned char b = imageData[idx + 2];

            if (r < minR) minR = r;
            if (g < minG) minG = g;
            if (b < minB) minB = b;
            if (r > maxR) maxR = r;
            if (g > maxG) maxG = g;
            if (b > maxB) maxB = b;
        }
    }

    double diffR = static_cast<double>(maxR - minR);
    double diffG = static_cast<double>(maxG - minG);
    double diffB = static_cast<double>(maxB - minB);

    return (diffR + diffG + diffB) / 3.0;
}

double computeEntropy(const unsigned char* imageData, int x, int y, int width, int height, int imageWidth, int channels) {
    std::array<int, 256> freqR = {0};
    std::array<int, 256> freqG = {0};
    std::array<int, 256> freqB = {0};
    int N = width * height;

    for (int j = y; j < y + height; ++j) {
        for (int i = x; i < x + width; ++i) {
            int idx = (j * imageWidth + i) * channels;
            freqR[imageData[idx]]++;
            freqG[imageData[idx + 1]]++;
            freqB[imageData[idx + 2]]++;
        }
    }

    auto calcEntropy = [N](const std::array<int, 256>& freq) {
        double entropy = 0.0;
        for (int i = 0; i < 256; ++i) {
            double p = static_cast<double>(freq[i]) / N;
            if (p > 0) {
                entropy -= p * std::log2(p);
            }
        }
        return entropy;
    };

    double entropyR = calcEntropy(freqR);
    double entropyG = calcEntropy(freqG);
    double entropyB = calcEntropy(freqB);

    return (entropyR + entropyG + entropyB) / 3.0;
}
