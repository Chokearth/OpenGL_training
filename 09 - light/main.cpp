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
        //    POSITION  / TexCoord / Normal
        // Front
        -.5f, -.5f, .5f, .0f, .0f, .0f, .0f, 1.0f, // Lower left
        -.5f, .5f, .5f, .0f, 2.f, .0f, .0f, 1.0f, // Upper left
        .5f, .5f, .5f, 2.f, 2.f, .0f, .0f, 1.0f, // Upper right
        .5f, -.5f, .5f, 2.f, .0f, .0f, .0f, 1.0f, // Lower right
        // Left
        -.5f, -.5f, -.5f, .0f, .0f, -1.0f, .0f, .0f, // Lower left
        -.5f, .5f, -.5f, .0f, 2.f, -1.0f, .0f, .0f, // Upper left
        -.5f, .5f, .5f, 2.f, 2.f, -1.0f, .0f, .0f, // Upper right
        -.5f, -.5f, .5f, 2.f, .0f, -1.0f, .0f, .0f, // Lower right
        // Back
        .5f, -.5f, -.5f, .0f, .0f, .0f, .0f, -1.0f, // Lower left
        .5f, .5f, -.5f, .0f, 2.f, .0f, .0f, -1.0f, // Upper left
        -.5f, .5f, -.5f, 2.f, 2.f, .0f, .0f, -1.0f, // Upper right
        -.5f, -.5f, -.5f, 2.f, .0f, .0f, .0f, -1.0f, // Lower right
        // Right
        .5f, -.5f, .5f, .0f, .0f, 1.0f, .0f, .0f, // Lower left
        .5f, .5f, .5f, .0f, 2.f, 1.0f, .0f, .0f, // Upper left
        .5f, .5f, -.5f, 2.f, 2.f, 1.0f, .0f, .0f, // Upper right
        .5f, -.5f, -.5f, 2.f, .0f, 1.0f, .0f, .0f, // Lower right
        // Top
        -.5f, .5f, .5f, .0f, .0f, .0f, 1.0f, .0f, // Lower left
        -.5f, .5f, -.5f, .0f, 2.f, .0f, 1.0f, .0f, // Upper left
        .5f, .5f, -.5f, 2.f, 2.f, .0f, 1.0f, .0f, // Upper right
        .5f, .5f, .5f, 2.f, .0f, .0f, 1.0f, .0f, // Lower right
        // Bottom
        -.5f, -.5f, -.5f, .0f, .0f, .0f, -1.0f, .0f, // Lower left
        -.5f, -.5f, .5f, .0f, 2.f, .0f, -1.0f, .0f, // Upper left
        .5f, -.5f, .5f, 2.f, 2.f, .0f, -1.0f, .0f, // Upper right
        .5f, -.5f, -.5f, 2.f, .0f, .0f, -1.0f, .0f, // Lower right
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

// Vertices of the light
GLfloat lightVertices[] = {
        -0.1f, -0.1f, 0.1f,
        -0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, -0.1f,
        0.1f, -0.1f, 0.1f,
        -0.1f, 0.1f, 0.1f,
        -0.1f, 0.1f, -0.1f,
        0.1f, 0.1f, -0.1f,
        0.1f, 0.1f, 0.1f
};

// Indices of the light
GLuint lightIndices[] = {
        0, 1, 2,
        0, 2, 3,
        0, 7, 4,
        0, 3, 7,
        3, 6, 7,
        3, 2, 6,
        2, 5, 6,
        2, 1, 5,
        1, 4, 5,
        1, 0, 4,
        4, 6, 5,
        4, 7, 6
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

    /* --- Shader program --- */

    // Load shader program
    Shader shaderProgram("shaders/basic.vert", "shaders/basic.frag");

    // Generate the VAO
    VAO VAO1;
    VAO1.bind();

    // Generate the VBO and EBO
    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    // Link VBO to VAO
    VAO1.linkVBO(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), 0);
    VAO1.linkVBO(VBO1, 1, 2, GL_FLOAT, 8 * sizeof(float), (void *) (3 * sizeof(float)));
    VAO1.linkVBO(VBO1, 2, 3, GL_FLOAT, 8 * sizeof(float), (void *) (5 * sizeof(float)));
    // Unbind
    VAO1.unbind();
    VBO1.unbind();
    EBO1.unbind();

    /* --- Light shader program --- */

    // Load shader program
    Shader lightShader("shaders/light.vert", "shaders/light.frag");

    // Generate the VAO
    VAO lightVAO;
    lightVAO.bind();

    // Generate the VBO and EBO
    VBO lightVBO(lightVertices, sizeof(lightVertices));
    EBO lightEBO(lightIndices, sizeof(lightIndices));

    // Link VBO to VAO
    lightVAO.linkVBO(lightVBO, 0, 3, GL_FLOAT, 3 * sizeof(float), 0);

    // Unbind
    lightVAO.unbind();
    lightVBO.unbind();
    lightEBO.unbind();

    /* --- light and cube matrix and parameter --- */

    // Color of the light
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    // Position of the light
    glm::vec3 lightPos = glm::vec3(.1f, 1.2, 0.0f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    // Position of the cube
    glm::vec3 cubePos = glm::vec3(.0f, .0f, .0f);
    glm::mat4 cubeModel = glm::mat4(1.0f);
    cubeModel = glm::translate(cubeModel, cubePos);

    // Put the data as Uniform
    lightShader.activate();
    glUniformMatrix4fv(glGetUniformLocation(lightShader.ID, "model"), 1, GL_FALSE, glm::value_ptr(lightModel));
    glUniform4f(glGetUniformLocation(lightShader.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    shaderProgram.activate();
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "model"), 1, GL_FALSE, glm::value_ptr(cubeModel));
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Texture
    Texture bricksTex("res/textures/Brick_Wall_019_basecolor.jpg", GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE);
    bricksTex.texUnit(shaderProgram, "tex0", 0);
    Texture bricksSpec("res/textures/Brick_Wall_019_ambientOcclusion.jpg", GL_TEXTURE_2D, 1, GL_RED, GL_UNSIGNED_BYTE);
    bricksTex.texUnit(shaderProgram, "tex1", 1);

    // Allow the triangle to be rendered on only one side
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Create the camera
    Camera camera(width, height, glm::vec3(0.704102f, .9f, 0.701461f));
    camera.orientation = glm::vec3(-0.72567f, -0.254602f, -0.639202f);

    while (!glfwWindowShouldClose(window)) {
        // Clear background
        glClearColor(.0f, .0f, .0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Read input to move the camera
        camera.inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        /* --- Default program --- */
        // Load the program to use
        shaderProgram.activate();
        VAO1.bind();
        glUniform3f(glGetUniformLocation(shaderProgram.ID, "camPos"), camera.position.x, camera.position.y, camera.position.z);
        // Add camMatrix Uniform
        camera.matrix(shaderProgram, "camMatrix");
        // Bind the texture
        bricksTex.bind();
        bricksSpec.bind();
        // Draw the triangles
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

        /* --- Light program --- */
        // Load the program to use
        lightShader.activate();
        lightVAO.bind();
        // Add camMatrix Uniform
        camera.matrix(lightShader, "camMatrix");
        // Draw the triangles
        glDrawElements(GL_TRIANGLES, sizeof(lightIndices) / sizeof(GLuint), GL_UNSIGNED_INT, 0);

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