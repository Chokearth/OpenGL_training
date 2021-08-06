//
// Created by chokearth on 8/2/21.
//

#pragma once

#include <glad/glad.h>
#include <map>
#include <any>
#include "../utils/utils.h"

// Type of value of the Uniform
enum UniformValueType {
    UniGLuint,
    UniGLfloat,
    UniGLint
};

class CShaderProgram {
private:
    // Uniform structure to store it in a map
    struct Uniform {
        GLint ID;
        UniformValueType type;
        std::any value;
    };
    // Image texture structure to store it in a map
    struct ImageTexture {
        GLuint texture;
        GLint level;
        GLboolean layered;
        GLint layer;
        GLenum access;
        GLenum format;
    };

    // Map all the uniforms values for the program
    std::map<std::string,Uniform> uniforms;
    // Map all the images textures for the program
    std::map<GLuint,ImageTexture> imagesTextures;
    // ID of the storage_buffer, 0 if none
    GLuint storage_buffer = 0;

    // Update the uniform associated to the program
    void updateUniform();
public:
    // ID of the program
    GLuint ID;

    // Constructor that just create the program
    CShaderProgram();

    // Load a shader
    void loadShader(char* path);

    // Add a uniform that will be updated at each call of the program
    void addUniform(std::string name, UniformValueType type, std::any value);
    // Change the value of a uniform
    void setUniform(std::string name, std::any value);

    // Add an image texture that will be bind at each call of the program
    void addImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access, GLenum format);
    // Set the storage buffer ID
    void setStorageBuffer(GLuint buffer);

    // Call the program
    void dispatch(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, bool wait = true);
    // Use the program
    void bind();
    // Reset the used program to 0
    void unbind();
};
