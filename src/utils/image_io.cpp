#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "image_io.hpp"
#include <algorithm>

bool isValidExtension(const std::string& filename) {
    std::string extension = filename.substr(filename.find_last_of('.') + 1);
    std::transform(extension.begin(), extension.end(), extension.begin(), ::tolower);
    return (extension == "png" || extension == "jpg" || extension == "jpeg");
}

std::string getFileExtension(const std::string& filename) {
    size_t dot = filename.find_last_of('.');
    if (dot == std::string::npos) return "";
    std::string ext = filename.substr(dot + 1);
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    return ext;
}

unsigned char* read_image(const char* filename, int* width, int* height, int* channels) {
    unsigned char* data = stbi_load(filename, width, height, channels, 0);
    if (data == nullptr) {
        fprintf(stderr, "Error loading image: %s\n", stbi_failure_reason());
    }
    return data;
}

void write_image(const std::string& filename, unsigned char* data, int width, int height, int channels) {
    std::string ext = getFileExtension(filename);
    bool success = false;

    if (ext == "png") {
        success = stbi_write_png(filename.c_str(), width, height, channels, data, width * channels);
    } else if (ext == "jpg" || ext == "jpeg") {
        success = stbi_write_jpg(filename.c_str(), width, height, channels, data, 100);
    } else {
        fprintf(stderr, "[ERROR] Format file output tidak didukung: %s\n", ext.c_str());
        return;
    }

    if (!success) {
        fprintf(stderr, "Error writing image: %s\n", stbi_failure_reason());
    }
}
