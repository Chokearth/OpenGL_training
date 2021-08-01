#include <iostream>
#include <cstring>
#include <chrono>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/Shader.h"
#include "graphics/VAO.h"
#include "graphics/EBO.h"
#include "graphics/Texture.h"

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
    GLuint slimeComputeShaderID;
    GLuint slimeProgramID;
    char *computeShader = 0;

    // Load and compile
    slimeComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);
    load_shader(&computeShader, "shaders/slime.comp");
    compile_shader(slimeComputeShaderID, computeShader);

    // shader program to manage the compute shader
    slimeProgramID = glCreateProgram();

    // Attach the compute shader to the program
    glAttachShader(slimeProgramID, slimeComputeShaderID);
    glLinkProgram(slimeProgramID);
    glDeleteShader(slimeComputeShaderID);

    /* --- Diffuse compute shader --- */
    GLuint DiffuseComputeShaderID;
    GLuint diffuseProgramID;
    computeShader = 0;

    // Load and compile
    DiffuseComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);
    load_shader(&computeShader, "shaders/diffuse.comp");
    compile_shader(DiffuseComputeShaderID, computeShader);

    // shader program to manage the compute shader
    diffuseProgramID = glCreateProgram();

    // Attach the compute shader to the program
    glAttachShader(diffuseProgramID, DiffuseComputeShaderID);
    glLinkProgram(diffuseProgramID);
    glDeleteShader(DiffuseComputeShaderID);

    /* --- Uniform --- */
    GLuint timeUniIDS = glGetUniformLocation(slimeProgramID, "time");
    GLuint deltaTimeUniIDS = glGetUniformLocation(slimeProgramID, "deltaTime");
    GLuint numAgentsUniIDS = glGetUniformLocation(slimeProgramID, "numAgents");
    GLuint widthUniIDS = glGetUniformLocation(slimeProgramID, "width");
    GLuint heightUniIDS = glGetUniformLocation(slimeProgramID, "height");

    GLuint timeUniIDD = glGetUniformLocation(diffuseProgramID, "time");
    GLuint deltaTimeUniIDD = glGetUniformLocation(diffuseProgramID, "deltaTime");
    GLuint widthUniIDD = glGetUniformLocation(diffuseProgramID, "width");
    GLuint heightUniIDD = glGetUniformLocation(diffuseProgramID, "height");

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
        // Load the program
        glUseProgram(slimeProgramID);
        // Bind the texture and bind it as an output buffer to the shader
        trailMap.bind();
        glBindImageTexture(0, trailMap.ID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

        // Update the uniform
        GLuint time = duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        glUniform1ui(timeUniIDS, time);
        glUniform1f(deltaTimeUniIDS, static_cast<float>(time - prevTime) / 1000.0f);
        glUniform1ui(numAgentsUniIDS, numAgents);
        glUniform1i(widthUniIDS, width);
        glUniform1i(heightUniIDS, height);

        // Bind the agents buffer
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, agentsBuffer);

        // Compute the shader
        glDispatchCompute(numAgents / 100 + 1, 1, 1);
        // Wait for the computation to end
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // Unbind the agents buffer, texture and the program
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
        glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        trailMap.unbind();
        glUseProgram(0);

        /* --- Diffuse --- */
        // Load the program
        glUseProgram(diffuseProgramID);
        // Bind the texture and bind it as an output buffer to the shader
        trailMap.bind();
        diffusedTrailMap.bind();
        glBindImageTexture(0, trailMap.ID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindImageTexture(1, diffusedTrailMap.ID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

        // Update the uniform
        glUniform1ui(timeUniIDD, time);
        glUniform1f(deltaTimeUniIDD, static_cast<float>(time - prevTime) / 1000.0f);
        glUniform1i(widthUniIDD, width);
        glUniform1i(heightUniIDD, height);

        // Compute the shader
        glDispatchCompute(40, 40, 1);
        // Wait for the computation to end
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        // Unbind the agents buffer, texture and the program
        glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glBindImageTexture(1, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        diffusedTrailMap.unbind();
        trailMap.unbind();
        glUseProgram(0);

        // Update prevTime
        prevTime = time;

        // Swap the display buffer
        glfwSwapBuffers(window);
    }

    // Destroy everything
    glDeleteBuffers(1, &agentsBuffer);
    glDeleteProgram(diffuseProgramID);
    glDeleteProgram(slimeProgramID);
    VAO1.deleteFromGPU();
    VBO1.deleteFromGPU();
    EBO1.deleteFromGPU();
    trailMap.deleteFromGPU();
    shaderProgram.deleteFromGPU();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}