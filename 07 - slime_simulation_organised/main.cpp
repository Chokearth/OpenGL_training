#include <iostream>
#include <cstring>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "openGL/Shader.h"
#include "openGL/VAO.h"
#include "openGL/EBO.h"
#include "openGL/Texture.h"
#include "openGL/CShaderProgram.h"

// Width of the texture
int width = 400;
// Height of the texture
int height = 400;
// Factor size of the window compared to the quad
float factor = 2.0f;
// Number of repetition of the texture
float textureFactor = 2.0f;

// List of vertices
GLfloat vertices[] = {
        //    POSITION    / TexCoord
        -1.0f, -1.0f, .0f, .0f, .0f, // Lower left
        1.0f, -1.0f, .0f, textureFactor, .0f, // Lower right
        -1.0f, 1.0f, .0f, .0f, textureFactor, // Upper left
        1.0f, 1.0f, .0f, textureFactor, textureFactor// Upper right
};

// List of indices
GLuint indices[] = {
        0, 1, 2,
        1, 2, 3
};

// vector 2 struct compatible with the shader
struct alignas(8) vec2 {
    float x, y;
};

// Number of agent
const uint numAgents = 200000;
// Agent structure
struct Agent {
    vec2 position;
    float angle;
};

// Generate a random float between min and max
float randFloat(float min, float max) {
    return min + static_cast<float> (rand()) / static_cast<float>(RAND_MAX / (max - min));
}

int main() {
    // Init rand
    srand(time(NULL));

    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // Tell GLFW the profile to use
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the windows
    GLFWwindow *window = glfwCreateWindow(width * factor, height * factor, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    gladLoadGL();

    //Specify the viewport of OpenGL in the Window
    glViewport(0, 0, width * factor, height * factor);

    // Load shader program
    Shader shaderProgram("shaders/basic.vert", "shaders/basic.frag");

    // Generate the VAO
    VAO VAO1;
    VAO1.bind();

    // Generate the VBO and EBO
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    // Link VBO to VAO
    VAO1.linkVBO(VBO1, 0, 3, GL_FLOAT, 5 * sizeof(float), 0);
    VAO1.linkVBO(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    // Unbind
    VAO1.unbind();
    VBO1.unbind();
    EBO1.unbind();

    // Texture
    Texture trailMap(width, height, GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA32F, GL_FLOAT);
    Texture diffusedTrailMap(width, height, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA32F, GL_FLOAT);
    diffusedTrailMap.texUnit(shaderProgram, "tex0", 0);

    /* --- Slime compute shader --- */
    CShaderProgram slimeProgram;
    slimeProgram.loadShader("shaders/slime.comp");
    slimeProgram.addImageTexture(0, trailMap.ID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

    /* --- Diffuse compute shader --- */
    CShaderProgram diffuseProgram;
    diffuseProgram.loadShader("shaders/diffuse.comp");
    diffuseProgram.addImageTexture(0, trailMap.ID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    diffuseProgram.addImageTexture(1, diffusedTrailMap.ID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    /* --- Uniform --- */
    slimeProgram.addUniform("time", UniGLuint, 0);
    slimeProgram.addUniform("deltaTime", UniGLfloat, 0.0f);
    slimeProgram.addUniform("numAgents", UniGLuint, numAgents);
    slimeProgram.addUniform("width", UniGLint, width);
    slimeProgram.addUniform("height", UniGLint, height);

    diffuseProgram.addUniform("time", UniGLuint, 0);
    diffuseProgram.addUniform("deltaTime", UniGLfloat, 0.0f);
    diffuseProgram.addUniform("width", UniGLint, width);
    diffuseProgram.addUniform("height", UniGLint, height);

    /* --- Structured buffer --- */

    // Generate agents
    Agent agents[numAgents];
    for (int i = 0; i < numAgents; i++) {
        agents[i] = {
                {
                        static_cast<float>(width / 2),
                        static_cast<float>(height / 2)
                },
                randFloat(-M_PI, M_PI)
        };
    }

    // Create the buffer
    GLuint agentsBuffer;
    glGenBuffers(1, &agentsBuffer);
    // Bind the buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, agentsBuffer);
    // Transfer the data to the buffer
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(agents), agents, GL_STATIC_DRAW);
    // Bind the buffer to a location
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, agentsBuffer);
    // Unbind the buffer
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    // Link to the program
    slimeProgram.setStorageBuffer(agentsBuffer);

    // To get the delta time
    GLuint prevTime = duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count();
    while (!glfwWindowShouldClose(window)) {
        // Poll window event
        glfwPollEvents();

        // Clear background
        glClearColor(.0f, .0f, 1.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Load the program to use
        shaderProgram.activate();
        VAO1.bind();

        // Bind the texture
        diffusedTrailMap.bind();

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Copy diffused trail to the trail texture
        glCopyImageSubData(diffusedTrailMap.ID, GL_TEXTURE_2D, 0, 0, 0, 0, trailMap.ID, GL_TEXTURE_2D, 0, 0, 0, 0,
                           width, height, 1);

        /* --- Slime simulation --- */
        // Update the uniform
        GLuint time = duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        slimeProgram.setUniform("time", time);
        slimeProgram.setUniform("deltaTime", static_cast<float>(time - prevTime) / 1000.0f);

        // Compute the shader
        slimeProgram.dispatch(numAgents / 100 + 1, 1, 1);

        /* --- Diffuse --- */
        // Update the uniform
        diffuseProgram.setUniform("time", time);
        diffuseProgram.setUniform("deltaTime", static_cast<float>(time - prevTime) / 1000.0f);

        // Compute the shader
        diffuseProgram.dispatch(40, 40, 1);

        // Update prevTime
        prevTime = time;

        // Swap the display buffer
        glfwSwapBuffers(window);
    }

    // Destroy everything
    glDeleteBuffers(1, &agentsBuffer);
    glDeleteProgram(diffuseProgram.ID);
    glDeleteProgram(slimeProgram.ID);
    VAO1.deleteFromGPU();
    VBO1.deleteFromGPU();
    EBO1.deleteFromGPU();
    trailMap.deleteFromGPU();
    shaderProgram.deleteFromGPU();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}