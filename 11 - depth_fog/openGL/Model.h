//
// Created by chokearth on 8/4/21.
//

#pragma once

#include <json/json.h>
#include "Mesh.h"
#include "../utils/utils.h"

class Model {
public:
    // Load the model
    Model(const char* file, glm::mat4 initialTransform = glm::mat4(1.0f));

    // Draw the model
    void draw(Shader& shader, Camera& camera);

private:
    // Path to the file
    const char* file;
    std::vector<unsigned char> data;
    // File as a JSON format
    nlohmann::json json;

    std::vector<Mesh> meshes;
    std::vector<glm::vec3> translationsMeshes;
    std::vector<glm::quat> rotationsMeshes;
    std::vector<glm::vec3> scalesMeshes;
    std::vector<glm::mat4> matricesMeshes;

    std::vector<std::string> loadedTexName;
    std::vector<Texture> loadedTex;

    void loadMesh(unsigned int indMesh);

    // Traverse a node and call himself recursively for the child node
    void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

    // Convert the binary to usable data
    std::vector<unsigned char> getData();
    std::vector<float> getFloats(nlohmann::json accessor);
    std::vector<GLuint> getIndices(nlohmann::json accessor);

    // Get the textures
    std::vector<Texture> getTextures();

    // Build the vertices with from the vectors
    std::vector<Vertex> assembleVertices (
            std::vector<glm::vec3> positions,
            std::vector<glm::vec3> normals,
            std::vector<glm::vec2> texUVs
            );

    // Convert the binary to vec
    std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
    std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
    std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};
