#ifndef QUADTREE_NODE_HPP
#define QUADTREE_NODE_HPP

#include "../utils/RGB.hpp"

class QuadtreeNode {
private:
    int x, y, width, height;
    RGB color;
    bool isLeaf;
    QuadtreeNode* children[4];

public:
    QuadtreeNode(int x, int y, int width, int height);

    ~QuadtreeNode();

    void setColor(const RGB& c) { color = c; }
    RGB getColor() const { return color; }
    bool isLeafNode() const { return isLeaf; }
    void setLeaf(bool leaf) { isLeaf = leaf; }

    int getX() const { return x; }
    int getY() const { return y; }
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    QuadtreeNode* getChild(int index) const { return children[index]; }
    void setChild(int index, QuadtreeNode* child) { children[index] = child; }

    void calculateAverageColor(const unsigned char* imageData, int imageWidth, int channels);
    bool isHomogeneous(const unsigned char* imageData, int imageWidth, int method, double threshold, int channels);
    void subdivide(const unsigned char* imageData, int imageWidth, int imageHeight, double threshold, int minBlockSize, int method, int channels);
    

    int countNodes() const;
    int getDepth() const;
};

#endif