//
// Created by chokearth on 7/30/21.
//

#include "Shader.h"

// Constructor that build the Shader Program from 3 different shaders
Shader::Shader(const char *vertexFile, const char *fragmentFile, const char * geometryFile) {
    // char * that will contain the shader source code
    char * vertexShaderSource;
    char * geometryShaderSource;
    char * fragmentShaderSource;

    // Load Vertex shader
    load_file(&vertexShaderSource, vertexFile);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    compile_shader(vertexShader, vertexShaderSource);

    // Load Geometry shader
    load_file(&geometryShaderSource, geometryFile);
    GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
    compile_shader(geometryShader, geometryShaderSource);

    // Load Fragment shader
    load_file(&fragmentShaderSource, fragmentFile);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compile_shader(fragmentShader, fragmentShaderSource);

    // Create the program
    ID = glCreateProgram();

    // Attach the shaders to the program
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, geometryShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // Error management
    GLint hasCompiled;
    glGetProgramiv(ID, GL_LINK_STATUS, &hasCompiled);
    if (hasCompiled == GL_FALSE) {
        char infoLog[1024];
        glGetProgramInfoLog(ID, 1024, NULL, infoLog);
        std::cerr << "SHADER_LINKING_ERROR for: PROGRAM\n" << infoLog << std::endl;
    }

    // Free memory by deleting the shader now that there are loaded in the program
    glDeleteShader(vertexShader);
    glDeleteShader(geometryShader);
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