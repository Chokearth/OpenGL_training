//
// Created by chokearth on 7/30/21.
//

#include "Shader.h"

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char *vertexFile, const char *fragmentFile) {
    // char * that will contain the shader source code
    char * vertexShaderSource;
    char * fragmentShaderSource;

    // Load Vertex shader
    load_file(&vertexShaderSource, vertexFile);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    compile_shader(vertexShader, vertexShaderSource);

    // Load Fragment shader
    load_file(&fragmentShaderSource, fragmentFile);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compile_shader(fragmentShader, fragmentShaderSource);

    // Create the program
    ID = glCreateProgram();

    // Attach the shaders to the program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // Free memory by deleting the shader now that there are loaded in the program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Activate the Shader Program
void Shader::activate() {
    glUseProgram(ID);
}

// Deletes the Shader Program
void Shader::deleteFromGPU() {
    glDeleteProgram(ID);
}