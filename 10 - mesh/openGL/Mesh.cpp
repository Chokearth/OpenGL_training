//
// Created by chokearth on 8/4/21.
//

#include "Mesh.h"

// Initialise the mesh's elements and create the VAO, VBO and EBO
Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices, std::vector<Texture> &textures) {
    Mesh::vertices = vertices;
    Mesh::indices = indices;
    Mesh::textures = textures;

    // Bind the VAO
    vao.bind();

    // Generate the VBO and EBO
    VBO VBO(vertices);
    EBO EBO(indices);

    // Link VBO to VAO
    vao.linkVBO(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), 0);
    vao.linkVBO(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void *) (3 * sizeof(float)));
    vao.linkVBO(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void *) (6 * sizeof(float)));
    vao.linkVBO(VBO, 3, 2, GL_FLOAT, sizeof(Vertex), (void *) (9 * sizeof(float)));
    // Unbind
    vao.unbind();
    VBO.unbind();
    EBO.unbind();
}

// Draw the mesh
void Mesh::draw(Shader &shader, Camera &camera,
                glm::mat4 matrix, glm::vec3 translation, glm::quat rotation, glm::vec3 scale) {
    // Bind the program and the VAO
    shader.activate();
    vao.bind();

    // Number of each type of texture
    unsigned int numDiffuse = 0;
    unsigned int numSpecular = 0;

    // Bind each texture
    for (unsigned int i = 0; i < textures.size(); i++) {
        std::string num;
        std::string type = textures[i].type;

        if (type == "diffuse") {
            num = std::to_string(numDiffuse++);
        } else if (type == "specular") {
            num = std::to_string(numSpecular++);
        }

        textures[i].texUnit(shader, (type + num).c_str(), i);
        textures[i].bind();
    }

    // Add the Uniforms
    glUniform3f(glGetUniformLocation(shader.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
    camera.matrix(shader, "camMatrix");

    // Create transformation matrix
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot = glm::mat4(1.0f);
    glm::mat4 sca = glm::mat4(1.0f);

    // Apply the input transformation
    trans = glm::translate(trans, translation);
    rot = glm::mat4_cast(rotation);
    sca = glm::scale(sca, scale);

    // Put the transformations matrices as Uniforms
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "translation"), 1, GL_FALSE, glm::value_ptr(trans));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "rotation"), 1, GL_FALSE, glm::value_ptr(rot));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "scale"), 1, GL_FALSE, glm::value_ptr(sca));
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

    // Draw the triangles
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}