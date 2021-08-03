//
// Created by chokearth on 7/30/21.
//

#pragma once

#include <glad/glad.h>
#include "VBO.h"

// Vertex Array Object
class VAO {
public:
    // ID of the VAO
    GLuint ID;

    // Constructor that generate a VAO
    VAO();

    // Links a VBO to the VAO using a certain layout
    void linkVBO(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    // Binds the VAO
    void bind();
    // Unbinds the VAO
    void unbind();
    // Deletes the VAO
    void deleteFromGPU();
};
