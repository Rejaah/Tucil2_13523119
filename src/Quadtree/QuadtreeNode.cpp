#include "QuadtreeNode.hpp"
#include "../utils/metrics.hpp"
#include <cstring>
#include <vector>

using namespace std;

QuadtreeNode::QuadtreeNode(int x, int y, int width, int height)
    : x(x), y(y), width(width), height(height), isLeaf(true) {
    memset(children, 0, sizeof(children));
}

QuadtreeNode::~QuadtreeNode() {
    for (auto& child : children) {
        delete child;
        child = nullptr;
    }
}

void QuadtreeNode::calculateAverageColor(const unsigned char* imageData, int imageWidth, int channels) {
    long long sumR = 0, sumG = 0, sumB = 0;
    int count = 0;

    for (int j = y; j < y + height; ++j) {
        for (int i = x; i < x + width; ++i) {
            int idx = (j * imageWidth + i) * channels;
            sumR += imageData[idx];
            sumG += imageData[idx + 1];
            sumB += imageData[idx + 2];
            ++count;
        }
    }

    if (count > 0) {
        setColor(RGB(
            static_cast<uint8_t>(sumR / count),
            static_cast<uint8_t>(sumG / count),
            static_cast<uint8_t>(sumB / count)
        ));
    }
}


bool QuadtreeNode::isHomogeneous(const unsigned char* imageData, int imageWidth, int method, double threshold, int channels) {
    double error = 0.0;
    switch (method) {
        case 1: error = computeVariance(imageData, x, y, width, height, imageWidth, channels); break;
        case 2: error = computeMAD(imageData, x, y, width, height, imageWidth, channels); break;
        case 3: error = computeMaxDiff(imageData, x, y, width, height, imageWidth, channels); break;
        case 4: error = computeEntropy(imageData, x, y, width, height, imageWidth, channels); break;
        default: throw std::invalid_argument("Invalid method");
    }

    return error <= threshold;
}

void QuadtreeNode::subdivide(const unsigned char* imageData, int imageWidth, int imageHeight,double threshold, int minBlockSize, int method, int channels) {
    if (width <= minBlockSize || height <= minBlockSize ||
        width <= 1 || height <= 1 ||
        width / 2 < minBlockSize || height / 2 < minBlockSize) {
        setLeaf(true);
        calculateAverageColor(imageData, imageWidth, channels);
        return;
    }

    if (isHomogeneous(imageData, imageWidth, method, threshold, channels)) {
        setLeaf(true);
        calculateAverageColor(imageData, imageWidth, channels);
        return;
    }

    setLeaf(false);

    int halfWidth = std::max(1, width / 2);
    int halfHeight = std::max(1, height / 2);

    children[0] = new QuadtreeNode(x, y, halfWidth, halfHeight);
    children[1] = new QuadtreeNode(x + halfWidth, y, width - halfWidth, halfHeight);
    children[2] = new QuadtreeNode(x, y + halfHeight, halfWidth, height - halfHeight);
    children[3] = new QuadtreeNode(x + halfWidth, y + halfHeight, width - halfWidth, height - halfHeight);

    for (auto& child : children) {
        child->subdivide(imageData, imageWidth, imageHeight, threshold, minBlockSize, method, channels);
    }
}

int QuadtreeNode::countNodes() const {
    if (isLeaf) {
        return 1;
    }

    int count = 1;
    for (const auto& child : children) {
        count += child->countNodes();
    }
    return count;
}

int QuadtreeNode::getDepth() const {
    if (isLeaf) {
        return 0;
    }

    int maxDepth = 0;
    for (const auto& child : children) {
        int childDepth = child->getDepth();
        if (childDepth > maxDepth) {
            maxDepth = childDepth;
        }
    }
    return maxDepth + 1;
}
