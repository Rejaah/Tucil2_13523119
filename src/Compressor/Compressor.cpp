#include "Compressor.hpp"
#include "../utils/image_io.hpp"
#include "../utils/stb_image.h"
#include "../utils/stb_image_write.h"
#include <iostream>
#include <chrono>
#include <thread>

#include <filesystem>
namespace fs = std::filesystem;

using namespace std;

struct ThresholdRange {
    double low;
    double high;
    int maxAttempt;
};

ThresholdRange getThresholdRange(int method) {
    switch (method) {
        case 1: return {0.0, 65025.0, 16};
        case 2: return {0.0, 127.5, 8};
        case 3: return {0.0, 255.0, 9};
        case 4: return {0.0, 8.0, 10};
        default: return {1.0, 10000.0, 12};
    }
}

Compressor::Compressor(const char* inputPath, const char* outputPath, int method, double threshold, int minBlockSize, double targetCompressionRatio) :
    root(nullptr),
    imageData(nullptr),
    inputPath(inputPath),
    outputPath(outputPath),
    height(0),
    width(0),
    channels(0),
    method(method),
    minBlockSize(minBlockSize),
    threshold(threshold),
    targetCompressionRatio(targetCompressionRatio),
    compressedSizeBytes(0),
    compressionSuccess(false) {}

Compressor::~Compressor() {
    if (root) {
        delete root;
    }
    freeImageData();
}

void Compressor::compressImage() {
    cout << "[LOG] Starting compression..." << endl;
    cout << "[LOG] Reading image: " << inputPath << endl;
    imageData = read_image(inputPath, &width, &height, &channels);
    if (!imageData) {
        cerr << "Failed to load image: " << inputPath << endl;
        return;
    }

    cout << "[LOG] Image loaded: " << width << "x" << height << " channels: " << channels << endl;
    originalSizeBytes = fs::file_size(inputPath); 

    auto start = std::chrono::high_resolution_clock::now(); 
    if (targetCompressionRatio > 0.0) {
        minBlockSize = 4;
        ThresholdRange range = getThresholdRange(method);
        double low = range.low;
        double high = range.high;
        int maxAttempt = range.maxAttempt;
        const double epsilon = 1e-3;

        double bestThreshold = -1;
        double bestRatio = 1.0;

        int attempts = 1;
        while (low <= high && attempts < maxAttempt + 1) {
            double progress = static_cast<double>(attempts) / maxAttempt;
            cout << "[LOG] Loading: " << static_cast<int>(progress * 100) << "%\r" << flush;
            attempts++;
            threshold = (low + high) / 2.0;
            if (root) delete root;

            buildQuadtree();
            saveCompressedImage();
            compressionRatio = static_cast<double>(compressedSizeBytes) / originalSizeBytes;
            double compressionPercent = 1.0 - compressionRatio;

            // cout << "[DEBUG] threshold=" << threshold
            //      << ", ratio=" << compressionRatio
            //      << ", penyusutan=" << compressionPercent * 100 << "%" << endl;

            if (compressionPercent >= targetCompressionRatio) {
                bestThreshold = threshold;
                bestRatio = compressionRatio;
                high = threshold - epsilon;
            } else {
                low = threshold + epsilon;
            }
        }

        if (bestThreshold >= 0.0) {
            threshold = bestThreshold;
            if (root) delete root;
            buildQuadtree();
            saveCompressedImage();
            compressionRatio = bestRatio;
        } else {
            cerr << "[WARNING] Tidak ditemukan threshold yang memenuhi target penyusutan." << endl;
        }
    } else {
        buildQuadtree();
        saveCompressedImage();
        compressionRatio = static_cast<double>(compressedSizeBytes) / originalSizeBytes;
    }

    auto end = chrono::high_resolution_clock::now();
    executionTime = chrono::duration<double>(end - start).count();
    analyzeImage();
}

void Compressor::analyzeImage() {
    if (!root) return;
    cout << fixed << setprecision(4);
    cout << "\n=== Statistik Kompresi ===" << endl;
    cout << "Waktu eksekusi:           " << executionTime << " detik" << endl;
    cout << "Ukuran gambar (asli):     " << originalSizeBytes << " bytes" << endl;
    cout << "Ukuran gambar (kompresi): " << compressedSizeBytes << " bytes" << endl;
    cout << "Persentase kompresi:      " << (100.0 * (1.0 - compressionRatio)) << "%" << endl;
    cout << "Kedalaman pohon:          " << root->getDepth() << endl;
    cout << "Jumlah simpul pohon:      " << root->countNodes() << endl;

    if (compressionSuccess) {
        cout << "Gambar disimpan di:       " << outputPath << endl;
    } 

    cout << "==========================" << endl;
}

void Compressor::fillBlock(unsigned char* data, int imageWidth, const QuadtreeNode* node) const {
    RGB color = node->getColor();
    for (int j = node->getY(); j < node->getY() + node->getHeight(); ++j) {
        for (int i = node->getX(); i < node->getX() + node->getWidth(); ++i) {
            int idx = (j * imageWidth + i) * channels;
            data[idx]     = color.getR();
            data[idx + 1] = color.getG();
            data[idx + 2] = color.getB();
            if (channels == 4) data[idx + 3] = 255;
        }
    }
}

void Compressor::renderQuadtree(unsigned char* data, int imageWidth, const QuadtreeNode* node) const {
    if (node->isLeafNode()) {
        fillBlock(data, imageWidth, node);
    } else {
        for (int i = 0; i < 4; ++i) {
            if (node->getChild(i)) {
                renderQuadtree(data, imageWidth, node->getChild(i));
            }
        }
    }
}

void Compressor::saveCompressedImage() {
    if (!root) return;

    int size = width * height * channels;
    unsigned char* outputData = new unsigned char[size];
    std::fill(outputData, outputData + size, 0);
    
    renderQuadtree(outputData, width, root);

    write_image(outputPath, outputData, width, height, channels);
    
    if (fs::exists(outputPath)) {
        size_t tempSize = fs::file_size(outputPath);
        compressedSizeBytes = tempSize;
        if (tempSize <= originalSizeBytes) {
            compressionSuccess = true;
        } else {
            fs::remove(outputPath);
            cerr << "[WARNING] Kompresi tidak efisien. File tidak disimpan.\n";
            cout << "[INFO] Kompresi gambar tidak disimpan.\n";
            cout << "[SUGGESTION] Gunakan threshold atau minBlockSize yang lebih besar.\n";
            return;
        }
    } else {
        cerr << "[ERROR] File output tidak ditemukan setelah penyimpanan!\n";
        return;
    }
    delete[] outputData;
}
    
void Compressor::buildQuadtree() {
    root = new QuadtreeNode(0, 0, width, height);
    root->subdivide(imageData, width, height, threshold, minBlockSize, method, channels);
}

void Compressor::freeImageData() {
    if (imageData) {
        stbi_image_free(imageData);
        imageData = nullptr;
    }
}