#ifndef TEXTURELOADER_H
#define TEXTURELOADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <strstream>

#include "stb_image.h"

class TextureLoader {

public:

    static int loadTexture(std::string path) {
        return TextureLoader::_loadTextures(path);
    }   

    static std::vector<unsigned int> loadTextures(std::string base_path, std::vector<std::string> paths) {
        
        std::vector<unsigned int> textures = std::vector<unsigned int>(0);

        for (auto it = begin(paths); it != end (paths); ++it) {
            std::string final_path = base_path +*it;
            textures.push_back(TextureLoader::_loadTextures(final_path));
        }

        return textures;
    }

    static int _loadTextures(std::string path) {

        unsigned int texture;

        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // load and generate the texture
        int width, height, nrChannels;

        unsigned char *data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data && nrChannels == 3) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        } else if (data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        } else {
            std::cout << "Failed to load texture : " << path << std::endl;
            return -1;
        }

        stbi_image_free(data);

        return texture;
    }


};

#endif