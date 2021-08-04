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
    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures);

    // Draw the mesh
    void draw(Shader& shader, Camera& camera);
};
