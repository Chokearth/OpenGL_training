//
// Created by chokearth on 7/30/21.
//

#include "VAO.h"

// Constructor that generate a VAO
VAO::VAO() {
    glGenVertexArrays(1, &ID);
}

// Links a VBO to the VAO using a certain layout
void VAO::linkVBO(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset) {
    VBO.bind();
    glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
    glEnableVertexAttribArray(layout);
    VBO.unbind();
}

// Binds the VAO
void VAO::bind() {
    glBindVertexArray(ID);
}

// Unbinds the VAO
void VAO::unbind() {
    glBindVertexArray(0);
}

// Deletes the VAO
void VAO::deleteFromGPU() {
    glDeleteVertexArrays(1, &ID);
}