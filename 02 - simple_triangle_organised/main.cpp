#include <iostream>
#include <fstream>
#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math.h>
#include "graphics/Shader.h"
#include "graphics/VAO.h"
#include "graphics/EBO.h"

// List of vertices
GLfloat vertices[] = {
        -.5f, -.5f * float(sqrt(3)) / 3, .0f,
        .5f, -.5f * float(sqrt(3)) / 3, .0f,
        -.0f, .5f * float(sqrt(3)) * 2 / 3, .0f
};

// List of indices
GLuint indices[] = {
        0, 1, 2
};

int main() {
    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // Tell GLFW the profile to use
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the windows
    GLFWwindow *window = glfwCreateWindow(800, 800, "Simple triangle", NULL, NULL);
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
    VAO1.linkVBO(VBO1, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);
    // Unbind
    VAO1.unbind();
    VBO1.unbind();
    EBO1.unbind();

    // Instantiate the rotation uniform
    GLuint rotationUniID = glGetUniformLocation(shaderProgram.ID, "rotation");
    float rotation = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        // Clear background
        glClearColor(.0f, .0f, .0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Load the program to use
        shaderProgram.activate();
        VAO1.bind();

        // Set the value of the uniform
        glUniform1f(rotationUniID, rotation);
        rotation += 0.01f;

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // Swap the display buffer
        glfwSwapBuffers(window);

        // Poll window event
        glfwPollEvents();
    }

    // Destroy everything
    VAO1.deleteFromGPU();
    VBO1.deleteFromGPU();
    EBO1.deleteFromGPU();
    shaderProgram.deleteFromGPU();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
