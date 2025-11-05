
#include <glad/glad.h>
#include "imgloader.h"


// ---------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------------------------
unsigned int setupTexture(const std::string& filepath){
    // load and create a texture 
    // -------------------------
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); // all upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;

    std::string texturePath = getFullTexturePath(filepath);
    unsigned char *data = loadImg(texturePath, width, height, nrChannels);

    if (data)
    {
        for (int i = 0; i < 16; i += 4) {
            std::cout << "Pixel " << i / 4 << ": R=" << (int)data[i]
                        << " G=" << (int)data[i + 1]
                        << " B=" << (int)data[i + 2]
                        << " A=" << (int)data[i + 3] << std::endl;
        }
        
        //generate texture
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture in main()" << std::endl;
    }
    stbi_image_free(data);

    return texture;//texture id
};