//
// Created by chokearth on 7/30/21.
//

#include "Texture.h"

// Constructor that load the image and create the image
Texture::Texture(const char* image, GLenum texType, GLuint slot, GLenum format, GLenum pixelType) {
    type = texType;

    // Load image
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

    // Create the texture
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(type, ID);

    // Set parameters
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Copy the image to the texture
    glTexImage2D(type, 0, format, widthImg, heightImg, 0, format, pixelType, bytes);
    glGenerateMipmap(type);

    // Free space
    stbi_image_free(bytes);

    // Unbind
    glBindTexture(type, 0);
}

// Constructor that create an empty image
Texture::Texture(const int widthImg, const int heightImg, GLenum texType, GLuint slot, GLenum format, GLenum pixelType) {
    type = texType;

    // Create the texture
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(type, ID);

    // Set parameters
    glTexParameteri(type, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Make the image black
    glTexImage2D(type, 0, format, widthImg, heightImg, 0, GL_RGBA, pixelType, NULL);
    glGenerateMipmap(type);

    // Unbind
    glBindTexture(type, 0);
}

// Put the uniform of the Shader Program
void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit) {
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.activate();
    glUniform1i(texUni, unit);
}
// Bind the Texture
void Texture::bind() {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(type, ID);
}
// Unbind the Texture
void Texture::unbind() {
    glBindTexture(type, 0);
}
// Delete the Texture
void Texture::deleteFromGPU() {
    glDeleteTextures(1, &ID);
}