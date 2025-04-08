#ifndef IMAGE_IO_HPP
#define IMAGE_IO_HPP

unsigned char* read_image(const char* filename, int* width, int* height, int* channels);
void write_image(const char* filename, unsigned char* data, int width, int height, int channels);

#endif