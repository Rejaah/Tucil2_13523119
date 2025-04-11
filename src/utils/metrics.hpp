#ifndef METRICS_HPP
#define METRICS_HPP

double computeVariance(const unsigned char* imageData, int x, int y, int width, int height, int imagewidth, int channels);
double computeMAD(const unsigned char* imageData, int x, int y, int width, int height, int imagewidth, int channels);
double computeMaxDiff(const unsigned char* imageData, int x, int y, int width, int height, int imagewidth, int channels);
double computeEntropy(const unsigned char* imageData, int x, int y, int width, int height, int imagewidth, int channels);

#endif