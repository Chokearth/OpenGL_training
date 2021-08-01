#include <iostream>
#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "graphics/Shader.h"
#include "graphics/VAO.h"
#include "graphics/EBO.h"
#include "graphics/Texture.h"

// List of vertices
GLfloat vertices[] = {
        //    POSITION    / TexCoord
        -.5f, -.5f, .0f, .0f, .0f, // Lower left
        .5f, -.5f, .0f, 1.f, .0f, // Lower right
        -.5f, .5f, .0f, .0f, 1.f, // Upper left
        .5f, .5f, .0f, 1.f, 1.f// Upper right
};

// List of indices
GLuint indices[] = {
        0, 1, 2,
        1, 2, 3
};

int main() {
    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // Tell GLFW the profile to use
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the windows
    GLFWwindow *window = glfwCreateWindow(800, 800, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    gladLoadGL();

    //Specify the viewport of OpenGL in the Window
    glViewport(0, 0, 800, 800);

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
    Texture texture(800, 800, GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA32F, GL_FLOAT);
    texture.texUnit(shaderProgram, "tex0", 0);

    /* --- Compute shader --- */
    GLuint computeShaderID;
    GLuint csProgramID;
    char *computeShader = 0;

    // Load and compile
    computeShaderID = glCreateShader(GL_COMPUTE_SHADER);
    load_shader(&computeShader, "shaders/compute.comp");
    compile_shader(computeShaderID, computeShader);

    // Shader program to manage the compute shader
    csProgramID = glCreateProgram();

    // Attach the compute shader to the program
    glAttachShader(csProgramID, computeShaderID);
    glLinkProgram(csProgramID);
    glDeleteShader(computeShaderID);

    // Use the compute shader
    // Load the program
    glUseProgram(csProgramID);
    // Bind the texture and bind it as an output buffer to the shader
    texture.bind();
    glBindImageTexture(0, texture.ID, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    // Compute the shader
    glDispatchCompute(40, 40, 1);
    // Wait for the computation to end
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    // Unbind the texture and the program
    glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    texture.unbind();
    glUseProgram(0);

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
        texture.bind();

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap the display buffer
        glfwSwapBuffers(window);
    }

    // Destroy everything
    VAO1.deleteFromGPU();
    VBO1.deleteFromGPU();
    EBO1.deleteFromGPU();
    texture.deleteFromGPU();
    shaderProgram.deleteFromGPU();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}