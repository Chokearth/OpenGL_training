//
// Created by chokearth on 7/30/21.
//

#pragma once

#include <glad/glad.h>
#include "../utils/utils.h"

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
