//
// Created by chokearth on 7/30/21.
//

#pragma once

#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <cstring>

// Load a shader file to a char array
void load_shader(char ** shaderBuffer, const char *path);
// Compile a shader
void compile_shader(GLuint shaderID, const char *source);

class Shader {
public:
    // ID of the Shader Program
    GLuint ID;
    // Constructor that build the Shader Program from 2 different shaders
    Shader(const char * vertexFile, const char * fragmentFile);

    // Activate the Shader Program
    void activate();
    // Deletes the Shader Program
    void deleteFromGPU();
};
