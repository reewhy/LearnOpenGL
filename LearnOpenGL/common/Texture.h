//
// Created by 39353 on 3/29/2025.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include <iostream>
#include <ostream>

class Texture {
public:
    unsigned int ID;
    Texture(const char* path, int wrap, int filter, bool flip, int color_mode) {
        // Create texture
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);
        // Set blend and wrap options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(flip);

        // Get data from external picture
        unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, color_mode, width, height, 0, color_mode, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cout << "Texture failed to load at path: " << path << std::endl;
        }
        // Free the memory
        stbi_image_free(data);
    }

    void use(int text_name) {
        // Set the texture!!
        glActiveTexture(text_name);
        glBindTexture(GL_TEXTURE_2D, ID);
    }
};

#endif //TEXTURE_H
