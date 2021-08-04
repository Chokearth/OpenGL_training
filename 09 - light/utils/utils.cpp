//
// Created by chokearth on 8/2/21.
//

#include "utils.h"

// Load a file to a char array
void load_file(char ** shaderBuffer, const char *path) {
    // Open file
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file \"" << path << "\"" << std::endl;
        return;
    }

    // Convert to string
    std::string ret = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    // Convert to char *
    (*shaderBuffer) = new char[ret.length() + 1];
    std::strcpy((*shaderBuffer), ret.c_str());
}

// Compile a shader
void compile_shader(GLuint shaderID, const char *source) {
    // Compile
    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);

    // Error management
    GLint result = GL_FALSE;
    int infoLogLength = 1024;
    char shaderErrorMessage[1024] = {0};

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

    glGetShaderInfoLog(shaderID, infoLogLength, NULL, shaderErrorMessage);
    if (std::strlen(shaderErrorMessage) != 0)
        std::cerr << shaderErrorMessage << std::endl;
}