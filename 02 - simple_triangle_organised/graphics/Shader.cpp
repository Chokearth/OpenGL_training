//
// Created by chokearth on 7/30/21.
//

#include "Shader.h"

// Load a shader file to a char array
void load_shader(char ** shaderBuffer, const char *path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open shader file \"" << path << "\"" << std::endl;
        return;
    }

    std::string ret = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    (*shaderBuffer) = new char[ret.length() + 1];
    std::strcpy((*shaderBuffer), ret.c_str());
}

// Compile a shader
void compile_shader(GLuint shaderID, const char *source) {
    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);

    GLint result = GL_FALSE;
    int infoLogLength = 1024;
    char shaderErrorMessage[1024] = {0};

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

    glGetShaderInfoLog(shaderID, infoLogLength, NULL, shaderErrorMessage);
    if (std::strlen(shaderErrorMessage) != 0)
        std::cerr << shaderErrorMessage << std::endl;
}

// Constructor that build the Shader Program from 2 different shaders
Shader::Shader(const char *vertexFile, const char *fragmentFile) {
    char * vertexShaderSource;
    char * fragmentShaderSource;

    load_shader(&vertexShaderSource, vertexFile);
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    compile_shader(vertexShader, vertexShaderSource);

    load_shader(&fragmentShaderSource, fragmentFile);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    compile_shader(fragmentShader, fragmentShaderSource);

    ID = glCreateProgram();

    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

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