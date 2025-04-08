#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "image_io.hpp"

unsigned char* read_image(const char* filename, int* width, int* height, int* channels) {
    unsigned char* data = stbi_load(filename, width, height, channels, 3);
    if (data == nullptr) {
        fprintf(stderr, "Error loading image: %s\n", stbi_failure_reason());
    }
    return data;
}

void write_image(const char* filename, unsigned char* data, int width, int height, int channels) {
    if (stbi_write_png(filename, width, height, channels, data, width * channels) == 0) {
        fprintf(stderr, "Error writing image: %s\n", stbi_failure_reason());
    }
}