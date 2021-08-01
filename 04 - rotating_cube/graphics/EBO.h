//
// Created by chokearth on 7/30/21.
//

#pragma once

#include <glad/glad.h>

// Elements Buffer Object
class EBO {
public:
    // ID of the EBO
    GLuint ID;

    // Constructor that generates an Elements Buffer Object and links it to indices
    EBO(GLuint* indices, GLsizeiptr size);

    // Binds the EBO
    void bind();
    // Unbinds the EBO
    void unbind();
    // Deletes the EBO
    void deleteFromGPU();
};
