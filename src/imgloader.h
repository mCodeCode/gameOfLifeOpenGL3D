#pragma once

#include <iostream>
#include <string>
#include <filesystem>
#include <cstdio>
#include <windows.h>
#include <algorithm>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// Replace backslashes with forward slashes for consistency
std::string replaceSlashes(const std::string& input) {
    std::string result = input;
    std::replace(result.begin(), result.end(), '\\', '/');
    return result;
}


// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// Get the directory of the current executable
std::string getExecutableDir() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string exePath(buffer);
    return exePath.substr(0, exePath.find_last_of("\\/"));
}


// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// Build full path to texture relative to executable
std::string getFullTexturePath(const std::string& relativePath) {
    return replaceSlashes(getExecutableDir() + "\\" + relativePath);
}

// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// Load texture and force RGBA output for OpenGL
unsigned char* loadImg(const std::string& filepath, int& width, int& height, int& channels) {
    // Check if file exists
    FILE* file = fopen(filepath.c_str(), "r");
    if (!file) {
        std::cerr << " loadImg: File not found or inaccessible: " << filepath << std::endl;
        return nullptr;
    }
    fclose(file);

    // Flip vertically for OpenGL
    stbi_set_flip_vertically_on_load(true);

    // Force RGBA output (4 channels)
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &channels, 4);

    // Log image info
    std::cout << " Trying to load texture from: " << filepath << std::endl;
    int x, y, comp;
    if (stbi_info(filepath.c_str(), &x, &y, &comp)) {
        std::cout << " Image info: " << x << "x" << y << ", " << comp << " original channels" << std::endl;
    } else {
        std::cerr << " stbi_info failed: " << stbi_failure_reason() << std::endl;
    }

    if (!data) {
        std::cerr << " loadImg: Failed to load image data: " << filepath << std::endl;
        return nullptr;
    }

    std::cout << " loadImg: Loaded texture (" << width << "x" << height << ", forced RGBA)" << std::endl;
    channels = 4; // Force output channel count
    return data;
}



// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
// Return OpenGL format for texture upload
inline GLenum getTextureFormat(int channels) {
    switch (channels) {
        case 1: return GL_RED;
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default: return GL_RGB;
    }
}