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

int width = 800;
int height = 800;
float factor = 1.0f;
float textureFactor = 1.0f;

GLfloat vertices[] = {
        //    POSITION    / TexCoord
        -1.0f, -1.0f , .0f, .0f, .0f, // Lower left
        1.0f, -1.0f , .0f, textureFactor, .0f, // Lower right
        -1.0f, 1.0f , .0f, .0f, textureFactor, // Upper left
        1.0f, 1.0f , .0f, textureFactor, textureFactor// Upper right
};

GLuint indices[] = {
        0, 1, 2,
        1, 2, 3
};

struct alignas(8) vec2 {
    float x, y;
};

const uint numAgents = 200000;
struct Agent {
    vec2 position;
    float angle;
};

float randFloat(float min, float max) {
    return min + static_cast<float> (rand()) / static_cast<float>(RAND_MAX/(max-min));
}

int main() {
    srand(time(NULL));

    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // Tell GLFW the profile to use
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the windows
    GLFWwindow *window = glfwCreateWindow(width*factor, height*factor, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    gladLoadGL();

    //Specify the viewport of OpenGL in the Window
    glViewport(0, 0, width*factor, height*factor);

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
    VAO1.linkVBO(VBO1, 1, 2, GL_FLOAT, 5 * sizeof(float), (void*)(3*sizeof(float)));
    // Unbind
    VAO1.unbind();
    VBO1.unbind();
    EBO1.unbind();

    // Texture
    Texture trailMap(width, height, GL_TEXTURE_2D, GL_TEXTURE1, GL_RGBA32F, GL_FLOAT);
//    trailMap.texUnit(shaderProgram, "tex0", 0);
Texture diffusedTrailMap(width, height, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA32F, GL_FLOAT);
    diffusedTrailMap.texUnit(shaderProgram, "tex0", 0);

    /* --- Slime compute shader --- */
    GLuint slimeComputeShaderID;
    GLuint slimeProgramID;
    char * computeShader = 0;

    // Load and compile
    slimeComputeShaderID = glCreateShader(GL_COMPUTE_SHADER);
    load_shader(&computeShader, "shaders/slime.comp");
    compile_shader(slimeComputeShaderID, computeShader);

    // shader program to manage the computeShader
    slimeProgramID = glCreateProgram();

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

    // shader program to manage the computeShader
    diffuseProgramID = glCreateProgram();

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
    Agent agents[numAgents];
    for (int i = 0; i < numAgents; i++) {
        agents[i] = {
                {
                    static_cast<float>(width/2),
                    static_cast<float>(height/2)
                },
                randFloat(-M_PI, M_PI)
        };
    }

    GLuint agentsBuffer;
    glGenBuffers(1, &agentsBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, agentsBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(agents), agents, GL_STATIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, agentsBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    // Wait a bit
//    uint wait_time = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//    uint wait_now;
//    do {
//        glfwPollEvents();
//        wait_now = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//    } while (wait_now - wait_time < 5000 && !glfwWindowShouldClose(window));

    GLuint prevTime = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        glClearColor(.0f, .0f, 1.0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        shaderProgram.activate();

        VAO1.bind();

        diffusedTrailMap.bind();

        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Reset texture
//        uint32_t  z = 0;
//        glClearTexImage(trailMap.ID, 0, GL_RGBA, GL_FLOAT, &z);
glCopyImageSubData(diffusedTrailMap.ID, GL_TEXTURE_2D, 0, 0, 0, 0, trailMap.ID, GL_TEXTURE_2D, 0, 0, 0, 0, width, height, 1);

        // Use the computeShader
        glAttachShader(slimeProgramID, slimeComputeShaderID);
        glLinkProgram(slimeProgramID);
        glUseProgram(slimeProgramID);
        trailMap.bind();
        glBindImageTexture(0, trailMap.ID, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

        GLuint time = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
        glUniform1ui(timeUniIDS, time);
        glUniform1f(deltaTimeUniIDS, static_cast<float>(time-prevTime)/1000.0f);
        glUniform1ui(numAgentsUniIDS, numAgents);
        glUniform1i(widthUniIDS, width);
        glUniform1i(heightUniIDS, height);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, agentsBuffer);

        glDispatchCompute(numAgents/100+1, 1, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

        glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        trailMap.unbind();
        glUseProgram(0);

        /* --- Diffuse --- */
        glUseProgram(diffuseProgramID);
        trailMap.bind();
        diffusedTrailMap.bind();
        glBindImageTexture(0, trailMap.ID, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
        glBindImageTexture(1, diffusedTrailMap.ID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

        glUniform1ui(timeUniIDD, time);
        glUniform1f(deltaTimeUniIDD, static_cast<float>(time-prevTime)/1000.0f);
        glUniform1i(widthUniIDD, width);
        glUniform1i(heightUniIDD, height);

        glDispatchCompute(40, 40, 1);
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
        glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        glBindImageTexture(1, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
        diffusedTrailMap.unbind();
        trailMap.unbind();
        glUseProgram(0);

        prevTime = time;

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