//
// Created by chokearth on 7/30/21.
//

#include "Texture.h"

// Constructor that load the image and create the image
Texture::Texture(const char* image, const char* texType, GLuint slot) {
    type = texType;

    // Load image
    int widthImg, heightImg, numColCh;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

    // Create the texture
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(GL_TEXTURE_2D, ID);

    // Set parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Determine the type with the number of channel
    if (numColCh == 4)
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                widthImg,
                heightImg,
                0,
                GL_RGBA,
                GL_UNSIGNED_BYTE,
                bytes
                );
    else if (numColCh == 3)
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                widthImg,
                heightImg,
                0,
                GL_RGB,
                GL_UNSIGNED_BYTE,
                bytes
                );
    else if (numColCh == 1)
        glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RGBA,
                widthImg,
                heightImg,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                bytes
                );
    else
        throw std::invalid_argument("Automatic Texture type recognition failed");

    // Generates MipMaps
    glGenerateMipmap(GL_TEXTURE_2D);

    // Free space
    stbi_image_free(bytes);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
}

// Constructor that create an empty image
Texture::Texture(const int widthImg, const int heightImg, const char* texType, GLuint slot, GLenum format, GLenum pixelType) {
    type = texType;

    // Create the texture
    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    unit = slot;
    glBindTexture(GL_TEXTURE_2D, ID);

    // Set parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Make the image black
    glTexImage2D(GL_TEXTURE_2D, 0, format, widthImg, heightImg, 0, GL_RGBA, pixelType, NULL);
    glGenerateMipmap(GL_TEXTURE_2D);

    // Unbind
    glBindTexture(GL_TEXTURE_2D, 0);
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
    glBindTexture(GL_TEXTURE_2D, ID);
}
// Unbind the Texture
void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}
// Delete the Texture
void Texture::deleteFromGPU() {
    glDeleteTextures(1, &ID);
}