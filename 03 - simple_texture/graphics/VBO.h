//
// Created by chokearth on 7/30/21.
//

#pragma once

#include <glad/glad.h>

// Vertex Buffer Object
class VBO {
public:
    // ID of the VBO
    GLuint ID;

    // Constructor that generates a Vertex Buffer Object and links it to vertices
    VBO(GLfloat* vertices, GLsizeiptr size);

    // Binds the VBO
    void bind();
    // Unbinds the VBO
    void unbind();
    // Deletes the VBO
    void deleteFromGPU();
};
