//
// Created by chokearth on 8/2/21.
//

#include "CShaderProgram.h"

// Constructor that just create the program
CShaderProgram::CShaderProgram() {
    // shader program to manage the compute shader
    ID = glCreateProgram();
}

// Load a shader
void CShaderProgram::loadShader(char *path) {
    GLuint shaderID;
    char *computeShader = 0;

    // Load and compile
    shaderID = glCreateShader(GL_COMPUTE_SHADER);
    load_file(&computeShader, path);
    compile_shader(shaderID, computeShader);

    // Attach the compute shader to the program
    glAttachShader(ID, shaderID);
    glLinkProgram(ID);
    glDeleteShader(shaderID);
}

// Add a uniform that will be updated at each call of the program
void CShaderProgram::addUniform(std::string name, UniformValueType type, std::any value) {
    uniforms[name] = {
            glGetUniformLocation(ID, name.c_str()),
            type,
            value
    };
}

// Change the value of a uniform
void CShaderProgram::setUniform(std::string name, std::any value) {
    uniforms[name].value = value;
}

// Update the uniform associated to the program
void CShaderProgram::updateUniform() {
    for (const auto& [key, value] : uniforms) {
        switch (value.type) {
            case UniGLfloat:
                glUniform1f(value.ID, std::any_cast<float>(value.value));
                break;
            case UniGLuint:
                glUniform1ui(value.ID, std::any_cast<uint>(value.value));
                break;
            case UniGLint:
                glUniform1i(value.ID, std::any_cast<int>(value.value));
                break;
        }
    }
}

// Add an image texture that will be bind at each call of the program
void
CShaderProgram::addImageTexture(GLuint unit, GLuint texture, GLint level, GLboolean layered, GLint layer, GLenum access,
                                GLenum format) {
    imagesTextures[unit] = {
            texture, level, layered, layer, access, format
    };
}

// Set the storage buffer ID
void CShaderProgram::setStorageBuffer(GLuint buffer) {
    storage_buffer = buffer;
}

// Call the program
void CShaderProgram::dispatch(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z, bool wait) {
    // Use the program
    bind();

    if (storage_buffer != 0) {
        // Bind the agents buffer
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, storage_buffer);
    }

    updateUniform();

    // Bind images textures
    for (const auto& [key, value] : imagesTextures) {
        glBindImageTexture(key, value.texture, value.level, value.layered, value.layer, value.access, value.format);
    }

    // Compute the shader
    glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
    if (wait) {
        // Wait for the computation to end
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }

    // Unbind images textures
    for (const auto& [key, value] : imagesTextures) {
        glBindImageTexture(key, 0, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    }
    if (storage_buffer != 0) {
        // Unbind the agents buffer, texture and the program
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    // Unbind the program
    unbind();
}

// Use the program
void CShaderProgram::bind() {
    glUseProgram(ID);
}
// Reset the used program to 0
void CShaderProgram::unbind() {
    glUseProgram(0);
}