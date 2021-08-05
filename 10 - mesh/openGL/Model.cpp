//
// Created by chokearth on 8/4/21.
//

#include "Model.h"

// Load the model
Model::Model(const char *file) {
    // Load JSON
    std::string text = get_file_contents(file);
    json = nlohmann::json::parse(text);

    Model::file = file;
    // Get the binary data
    data = getData();

    // Traverse all nodes
    traverseNode(0);
}

// Draw the model
void Model::draw(Shader &shader, Camera &camera) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].draw(shader, camera, matricesMeshes[i]);
    }
}

// Load a mesh and add it to the meshes vector
void Model::loadMesh(unsigned int indMesh) {
    // Get accessors indices
    unsigned int posAccInd    = json["meshes"][indMesh]["primitives"][0]["attributes"]["POSITION"];
    unsigned int normalAccInd = json["meshes"][indMesh]["primitives"][0]["attributes"]["NORMAL"];
    unsigned int texAccInd    = json["meshes"][indMesh]["primitives"][0]["attributes"]["TEXCOORD_0"];
    unsigned int indAccInd    = json["meshes"][indMesh]["primitives"][0]["indices"];

    // Get all the component with the accessors
    std::vector<float> posVec        = getFloats(json["accessors"][posAccInd]);
    std::vector<glm::vec3> positions = groupFloatsVec3(posVec);
    std::vector<float> normalVec     = getFloats(json["accessors"][normalAccInd]);
    std::vector<glm::vec3> normals   = groupFloatsVec3(normalVec);
    std::vector<float> texVec        = getFloats(json["accessors"][texAccInd]);
    std::vector<glm::vec2> texUVs    = groupFloatsVec2(texVec);

    // Build the mesh data with all the component
    std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
    std::vector<GLuint> indices = getIndices(json["accessors"][indAccInd]);
    std::vector<Texture> textures = getTextures();

    // Construct the mesh and push it
    meshes.push_back(Mesh(vertices, indices, textures));
}

// Traverse a node and call himself recursively for the child node
void Model::traverseNode(unsigned int nextNode, glm::mat4 matrix) {
    // Get JSON description of the current node
    nlohmann::json node = json["nodes"][nextNode];

    // Load the translation vector
    glm::vec3 translation = glm::vec3(0.0f, 0.0f, 0.0f);
    if (node.find("translation") != node.end()) {
        float transValues[3];
        for (unsigned int i = 0; i < node["translation"].size(); i++)
            transValues[i] = node["translation"][i];
        translation = glm::make_vec3(transValues);
    }

    // Load the rotation quaternion
    glm::quat rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    if (node.find("rotation") != node.end()) {
        float rotValues[4] = {
                node["rotation"][3],
                node["rotation"][0],
                node["rotation"][1],
                node["rotation"][2],
                };
        rotation = glm::make_quat(rotValues);
    }

    // Load the scale vector
    glm::vec3 scale = glm::vec3(1.0f, 1.0f, 1.0f);
    if (node.find("scale") != node.end()) {
        float scaleValues[3];
        for (unsigned int i = 0; i < node["scale"].size(); i++)
            scaleValues[i] = node["scale"][i];
        scale = glm::make_vec3(scaleValues);
    }

    // Load the model matrix
    glm::mat4 matNode = glm::mat4(1.0f);
    if (node.find("matrix") != node.end()) {
        float matValues[16];
        for (unsigned int i = 0; i < node["matrix"].size(); i++)
            matValues[i] = node["matrix"][i];
        matNode = glm::make_mat4(matValues);
    }

    // Create default transformation matrices
    glm::mat4 trans = glm::mat4(1.0f);
    glm::mat4 rot = glm::mat4(1.0f);
    glm::mat4 sca = glm::mat4(1.0f);

    // Apply the transformations on the matrices
    trans = glm::translate(trans, translation);
    rot = glm::mat4_cast(rotation);
    sca = glm::scale(sca, scale);

    // Multiply all matrices together
    glm::mat4 matNextNode = matrix * matNode * trans * rot * sca;

    // If the node contain a mesh, load it
    if (node.find("mesh") != node.end()) {
        translationsMeshes.push_back(translation);
        rotationsMeshes.push_back(rotation);
        scalesMeshes.push_back(scale);
        matricesMeshes.push_back(matNextNode);

        loadMesh(node["mesh"]);
    }

    // Explore children
    if (node.find("children") != node.end()) {
        for (unsigned int i = 0; i < node["children"].size(); i++)
            traverseNode(node["children"][i], matNextNode);
    }
}

// Load the binary file
std::vector<unsigned char> Model::getData() {

    // Raw text
    std::string bytesText;
    // Get the uri of the .bin file
    std::string uri = json["buffers"][0]["uri"];

    // Get the path to the file
    std::string fileStr = std::string(file);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);
    // Get the content of the file
    bytesText = get_file_contents((fileDirectory + uri).c_str());

    // Transform the raw text to byte
    std::vector<unsigned char> data(bytesText.begin(), bytesText.end());
    return data;
}

// Build the vertices with from the vectors
std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals,
                                            std::vector<glm::vec2> texUVs) {
    std::vector<Vertex> vertices;

    for (int i = 0; i < positions.size(); i++) {
        vertices.push_back(
                Vertex{
                    positions[i],
                    normals[i],
                    glm::vec3(1.0f, 1.0f, 1.0f),
                    texUVs[i]
                }
                );
    }
    return vertices;
}

// Convert the binary to floats
std::vector<float> Model::getFloats(nlohmann::json accessor) {
    std::vector<float> floatVec;

    // Get properties from the accessor
    unsigned int buffViewInd = accessor.value("bufferView", 1);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    std::string type = accessor["type"];

    // Get properties from the bufferView
    nlohmann::json bufferView = json["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];

    // interpret the type
    unsigned int numPerVert;
    if (type == "SCALAR") numPerVert = 1;
    else if (type == "VEC2") numPerVert = 2;
    else if (type == "VEC3") numPerVert = 3;
    else if (type == "VEC4") numPerVert = 4;
    else throw std::invalid_argument("type is invalid");

    // Get over all bytes in the data
    unsigned int beginningOfData = byteOffset + accByteOffset;
    unsigned int lengthOfData = count * 4 * numPerVert;
    for (unsigned int i = beginningOfData; i < beginningOfData + lengthOfData; i) {
        unsigned char bytes[] = {data[i++], data[i++], data[i++], data[i++]};
        float value;
        std::memcpy(&value, bytes, sizeof(float));
        floatVec.push_back(value);
    }

    return floatVec;
}

// Get indices from binary
std::vector<GLuint> Model::getIndices(nlohmann::json accessor) {
    std::vector<GLuint> indices;

    // Get properties from the accessor
    unsigned int buffViewInd = accessor.value("bufferView", 1);
    unsigned int count = accessor["count"];
    unsigned int accByteOffset = accessor.value("byteOffset", 0);
    unsigned int componentType = accessor["componentType"];

    // Get properties from the bufferView
    nlohmann::json bufferView = json["bufferViews"][buffViewInd];
    unsigned int byteOffset = bufferView["byteOffset"];

    // Get indices with regard to their type: unsigned int, unsigned short, or short
    unsigned int beginningOfData = byteOffset + accByteOffset;
    if (componentType == 5125) { // unsigned int
        for (unsigned int i = beginningOfData; i < beginningOfData + count * 4; i) {
            unsigned char bytes[] = {data[i++], data[i++], data[i++], data[i++]};
            unsigned int value;
            std::memcpy(&value, bytes, sizeof(unsigned int));
            indices.push_back((GLuint) value);
        }
    } else if (componentType == 5123) { // unsigned short
        for (unsigned int i = beginningOfData; i < beginningOfData + count * 2; i) {
            unsigned char bytes[] = {data[i++], data[i++]};
            unsigned short value;
            std::memcpy(&value, bytes, sizeof(unsigned short));
            indices.push_back((GLuint) value);
        }
    } else if (componentType == 5122) { // short
        for (unsigned int i = beginningOfData; i < beginningOfData + count * 2; i) {
            unsigned char bytes[] = {data[i++], data[i++]};
            short value;
            std::memcpy(&value, bytes, sizeof(unsigned short));
            indices.push_back((GLuint) value);
        }
    }

    return indices;
}

// Get the textures
std::vector<Texture> Model::getTextures() {
    std::vector<Texture> textures;

    std::string fileStr = std::string(file);
    std::string fileDirectory = fileStr.substr(0, fileStr.find_last_of('/') + 1);

    // Go over all image
    for (unsigned int i = 0; i < json["images"].size(); i++) {
        std::string texPath = json["images"][i]["uri"];

        // Check if the image as already been loaded
        bool skip = false;
        for (unsigned int j = 0; j < loadedTexName.size(); j++) {
            if (loadedTexName[j] == texPath) {
                textures.push_back(loadedTex[j]);
                skip = true;
                break;
            }
        }
        if (skip) continue;

        // Load the image
        if (texPath.find("baseColor") != std::string::npos) {
            Texture diffuse = Texture((fileDirectory + texPath).c_str(), "diffuse", loadedTex.size());
            textures.push_back(diffuse);

            loadedTexName.push_back(texPath);
            loadedTex.push_back(diffuse);
        } else if (texPath.find("metallicRoughness") != std::string::npos) {
            Texture specular = Texture((fileDirectory + texPath).c_str(), "specular", loadedTex.size());
            textures.push_back(specular);

            loadedTexName.push_back(texPath);
            loadedTex.push_back(specular);
        }
    }

    return textures;
}

// Get vec2 from binary
std::vector<glm::vec2> Model::groupFloatsVec2(std::vector<float> floatVec) {
    std::vector<glm::vec2> vectors;
    for (int i = 0; i < floatVec.size(); i) {
        vectors.push_back(glm::vec2(floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

// Get vec3 from binary
std::vector<glm::vec3> Model::groupFloatsVec3(std::vector<float> floatVec) {
    std::vector<glm::vec3> vectors;
    for (int i = 0; i < floatVec.size(); i) {
        vectors.push_back(glm::vec3(floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}

// Get vec4 from binary
std::vector<glm::vec4> Model::groupFloatsVec4(std::vector<float> floatVec) {
    std::vector<glm::vec4> vectors;
    for (int i = 0; i < floatVec.size(); i) {
        vectors.push_back(glm::vec4(floatVec[i++], floatVec[i++], floatVec[i++], floatVec[i++]));
    }
    return vectors;
}