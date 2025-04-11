#ifndef COMPRESSOR_HPP
#define COMPRESSOR_HPP

#include "QuadtreeNode.hpp"
#include "RGB.hpp"
#include <string>
using namespace std;

class Compressor {
    private:
        QuadtreeNode* root;
        unsigned char* imageData;
        const char* inputPath;
        const char* outputPath;
        int height, width, channels;
        int method; 
        int minBlockSize; 
        double threshold;
        double targetCompressionRatio;

        size_t originalSizeBytes;
        size_t compressedSizeBytes;
        double compressionRatio;
        double executionTime;

        bool compressionSuccess;
    public:
        Compressor(const char* inputPath, const char* outputPath, int method, double threshold, int minBlockSize, double targetCompressionRatio);
        ~Compressor();
        void compressImage();
        void analyzeImage();
        void saveCompressedImage();

        int getChannels() const { return channels; }
        void fillBlock(unsigned char* data, int imageWidth, const QuadtreeNode* node) const;
        void renderQuadtree(unsigned char* data, int imageWidth, const QuadtreeNode* node) const;


    private:
        void buildQuadtree();
        void freeImageData();
};

#endif 