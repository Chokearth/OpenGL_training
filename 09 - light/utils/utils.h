//
// Created by chokearth on 8/2/21.
//

#pragma once

#include <glad/glad.h>
#include <fstream>
#include <iostream>
#include <cstring>

// Load a file to a char array
void load_file(char ** shaderBuffer, const char *path);
// Compile a shader
void compile_shader(GLuint shaderID, const char *source);