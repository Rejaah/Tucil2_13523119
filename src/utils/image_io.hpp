#ifndef IMAGE_IO_HPP
#define IMAGE_IO_HPP

#include <string>
using namespace std;

bool isValidExtension(const string& filename);
string getFileExtension(const std::string& filename);
unsigned char* read_image(const char* filename, int* width, int* height, int* channels);
void write_image(const string& filename, unsigned char* data, int width, int height, int channels);

#endif