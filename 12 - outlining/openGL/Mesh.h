//
// Created by chokearth on 8/4/21.
//

#pragma once

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "Camera.h"
#include "Texture.h"

class Mesh {
public:
    // Elements that make the mesh
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    // Vertex Array Object
    VAO vao;

    // Initialise the mesh's elements and create the VAO, VBO and EBO
    Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures);

    // Draw the mesh
    void draw(Shader &shader, Camera &camera, glm::mat4 matrix = glm::mat4(1.0f),
              glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f),
              glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f), glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f));
};
