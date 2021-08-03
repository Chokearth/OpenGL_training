#include <iostream>
#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "openGL/Shader.h"
#include "openGL/VAO.h"
#include "openGL/EBO.h"
#include "openGL/Texture.h"
#include "openGL/Camera.h"

// List of vertices
GLfloat vertices[] = {
        //    POSITION    / TexCoord
        // Front
        -.5f, -.5f , .5f, .0f, .0f, // Lower left
        -.5f, .5f , .5f, .0f, 2.f, // Upper left
        .5f, .5f , .5f, 2.f, 2.f,// Upper right
        .5f, -.5f , .5f, 2.f, .0f, // Lower right
        // Left
        -.5f, -.5f , -.5f, .0f, .0f, // Lower left
        -.5f, .5f , -.5f, .0f, 2.f, // Upper left
        -.5f, .5f , .5f, 2.f, 2.f,// Upper right
        -.5f, -.5f , .5f, 2.f, .0f, // Lower right
        // Back
        .5f, -.5f , -.5f, .0f, .0f, // Lower left
        .5f, .5f , -.5f, .0f, 2.f, // Upper left
        -.5f, .5f , -.5f, 2.f, 2.f,// Upper right
        -.5f, -.5f , -.5f, 2.f, .0f, // Lower right
        // Right
        .5f, -.5f , .5f, .0f, .0f, // Lower left
        .5f, .5f , .5f, .0f, 2.f, // Upper left
        .5f, .5f , -.5f, 2.f, 2.f,// Upper right
        .5f, -.5f , -.5f, 2.f, .0f, // Lower right
        // Top
        -.5f, .5f , .5f, .0f, .0f, // Lower left
        -.5f, .5f , -.5f, .0f, 2.f, // Upper left
        .5f, .5f , -.5f, 2.f, 2.f,// Upper right
        .5f, .5f , .5f, 2.f, .0f, // Lower right
        // Bottom
        -.5f, -.5f , -.5f, .0f, .0f, // Lower left
        -.5f, -.5f , .5f, .0f, 2.f, // Upper left
        .5f, -.5f , .5f, 2.f, 2.f,// Upper right
        .5f, -.5f , -.5f, 2.f, .0f, // Lower right
};

// List of indices
GLuint indices[] = {
        // Front
        3, 2, 1,
        1, 0, 3,
        // Left
        7, 6, 5,
        5, 4, 7,
        // Back
        11, 10, 9,
        9, 8, 11,
        // Right
        15, 14, 13,
        13, 12, 15,
        // Top
        19, 18, 17,
        17, 16, 19,
        // Bottom
        23, 22, 21,
        21, 20, 23
};

const unsigned int width = 800;
const unsigned int height = 800;

int main() {
    // Initialize GLFW
    glfwInit();

    // Tell GLFW what version of OpenGL we use
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    // Tell GLFW the profile to use
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create the windows
    GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load GLAD
    gladLoadGL();

    //Specify the viewport of OpenGL in the Window
    glViewport(0, 0, width, height);

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
    Texture bricksTex("res/textures/bricks.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    bricksTex.texUnit(shaderProgram, "tex0", 0);

    // Allow the triangle to be rendered on only one side
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Create the camera
    Camera camera(width, height, glm::vec3(.0f, .0f, 2.0f));

    while (!glfwWindowShouldClose(window)) {
        // Clear background
        glClearColor(.0f, .0f, .0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Load the program to use
        shaderProgram.activate();
        VAO1.bind();

        // Read input to move the camera and affect the uniform
        camera.inputs(window);
        camera.matrix(45.0f, 0.1f, 100.0f, shaderProgram, "camMatrix");

        // Bind the texture
        bricksTex.bind();

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(GLuint), GL_UNSIGNED_INT, 0);

        // Swap the display buffer
        glfwSwapBuffers(window);

        // Poll window event
        glfwPollEvents();
    }

    // Destroy everything
    VAO1.deleteFromGPU();
    VBO1.deleteFromGPU();
    EBO1.deleteFromGPU();
    bricksTex.deleteFromGPU();
    shaderProgram.deleteFromGPU();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}