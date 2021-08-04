//
// Created by chokearth on 7/30/21.
//

#pragma once

#include <glad/glad.h>
#include <stb/stb_image.h>
#include "Shader.h"

class Texture {
public:
    // reference ID of the texture
    GLuint ID;
    // type of the texture
    GLenum type;
    // slot of the texture
    GLuint unit;

    // Constructor that load the image and create the image
    Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);
    // Constructor that create an empty image
    Texture(const int widthImg, const int heightImg, GLenum texType, GLuint slot, GLenum format, GLenum pixelType);

    // Put the uniform of the Shader Program
    void texUnit(Shader& shader, const char* uniform, GLuint unit);
    // Bind the Texture
    void bind();
    // Unbind the Texture
    void unbind();
    // Delete the Texture
    void deleteFromGPU();
};
