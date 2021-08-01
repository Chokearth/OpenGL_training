#include <iostream>
#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "graphics/Shader.h"
#include "graphics/VAO.h"
#include "graphics/EBO.h"
#include "graphics/Texture.h"

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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

    // Rotation variable
    float rotation = .0f;

    // Allow the triangle to be rendered on only one side
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    while (!glfwWindowShouldClose(window)) {
        // Clear background
        glClearColor(.0f, .0f, .0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Load the program to use
        shaderProgram.activate();
        VAO1.bind();

        // Create the 3D matrix
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);

        // Rotate the cube
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, .5f, 1.0f));
        // Configure the camera
        view = glm::translate(view, glm::vec3(.0f, .0f, -3.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)(width/height), .1f, 100.0f);

        // Pass the matrix as uniform
        GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        GLuint viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        GLuint projLoc = glGetUniformLocation(shaderProgram.ID, "proj");
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(proj));

        // Update the rotation
        rotation += 1.5f;

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