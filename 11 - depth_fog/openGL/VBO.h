//
// Created by chokearth on 7/30/21.
//

#pragma once

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

struct Vertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 textUV;
};

// Vertex Buffer Object
class VBO {
public:
    // ID of the VBO
    GLuint ID;

    // Constructor that generates a Vertex Buffer Object and links it to vertices
    VBO(std::vector<Vertex>& vertices);

    // Binds the VBO
    void bind();
    // Unbinds the VBO
    void unbind();
    // Deletes the VBO
    void deleteFromGPU();
};
