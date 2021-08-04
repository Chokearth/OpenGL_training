#include "openGL/Mesh.h"

// List of vertices
Vertex vertices[] = {
        // Front
        Vertex{glm::vec3(-.5f, -.5f, .5f) , glm::vec3(.0f  , .0f  , 1.0f) , glm::vec3(.0f, .0f, .0f), glm::vec2( .0f, .0f)},  // Lower left
        Vertex{glm::vec3(-.5f, .5f , .5f) , glm::vec3(.0f  , .0f  , 1.0f) , glm::vec3(.0f, .0f, .0f), glm::vec2( .0f, 2.0f)}, // Upper left
        Vertex{glm::vec3( .5f, .5f , .5f) , glm::vec3(.0f  , .0f  , 1.0f) , glm::vec3(.0f, .0f, .0f), glm::vec2(2.0f, 2.0f)}, // Upper right
        Vertex{glm::vec3( .5f, -.5f, .5f) , glm::vec3(.0f  , .0f  , 1.0f) , glm::vec3(.0f, .0f, .0f), glm::vec2(2.0f, .0f)},  // Lower right
        // Left
        Vertex{glm::vec3(-.5f, -.5f, -.5f), glm::vec3(-1.0f, .0f  , .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2( .0f, .0f)},  // Lower left
        Vertex{glm::vec3(-.5f, .5f , -.5f), glm::vec3(-1.0f, .0f  , .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2( .0f, 2.0f)}, // Upper left
        Vertex{glm::vec3(-.5f, .5f , .5f) , glm::vec3(-1.0f, .0f  , .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2(2.0f, 2.0f)}, // Upper right
        Vertex{glm::vec3(-.5f, -.5f, .5f) , glm::vec3(-1.0f, .0f  , .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2(2.0f, .0f)},  // Lower right
        // Back
        Vertex{glm::vec3( .5f, -.5f, -.5f), glm::vec3(.0f  , .0f  , -1.0f), glm::vec3(.0f, .0f, .0f), glm::vec2( .0f, .0f)},  // Lower left
        Vertex{glm::vec3( .5f, .5f , -.5f), glm::vec3(.0f  , .0f  , -1.0f), glm::vec3(.0f, .0f, .0f), glm::vec2( .0f, 2.0f)}, // Upper left
        Vertex{glm::vec3(-.5f, .5f , -.5f), glm::vec3(.0f  , .0f  , -1.0f), glm::vec3(.0f, .0f, .0f), glm::vec2(2.0f, 2.0f)}, // Upper right
        Vertex{glm::vec3(-.5f, -.5f, -.5f), glm::vec3(.0f  , .0f  , -1.0f), glm::vec3(.0f, .0f, .0f), glm::vec2(2.0f, .0f)},  // Lower right
        // Right
        Vertex{glm::vec3( .5f, -.5f, .5f) , glm::vec3(1.0f , .0f  , .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2( .0f, .0f)},  // Lower left
        Vertex{glm::vec3( .5f, .5f , .5f) , glm::vec3(1.0f , .0f  , .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2( .0f, 2.0f)}, // Upper left
        Vertex{glm::vec3( .5f, .5f , -.5f), glm::vec3(1.0f , .0f  , .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2(2.0f, 2.0f)}, // Upper right
        Vertex{glm::vec3( .5f, -.5f, -.5f), glm::vec3(1.0f , .0f  , .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2(2.0f, .0f)},  // Lower right
        // Top
        Vertex{glm::vec3(-.5f, .5f , .5f) , glm::vec3(.0f  , 1.0f , .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2( .0f, .0f)},  // Lower left
        Vertex{glm::vec3(-.5f, .5f , -.5f), glm::vec3(.0f  , 1.0f , .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2( .0f, 2.0f)}, // Upper left
        Vertex{glm::vec3( .5f, .5f , -.5f), glm::vec3(.0f  , 1.0f , .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2(2.0f, 2.0f)}, // Upper right
        Vertex{glm::vec3( .5f, .5f , .5f) , glm::vec3(.0f  , 1.0f , .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2(2.0f, .0f)},  // Lower right
        // Bottom
        Vertex{glm::vec3(-.5f, -.5f, -.5f), glm::vec3(.0f  , -1.0f, .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2( .0f, .0f)},  // Lower left
        Vertex{glm::vec3(-.5f, -.5f, .5f) , glm::vec3(.0f  , -1.0f, .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2( .0f, 2.0f)}, // Upper left
        Vertex{glm::vec3( .5f, -.5f, .5f) , glm::vec3(.0f  , -1.0f, .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2(2.0f, 2.0f)}, // Upper right
        Vertex{glm::vec3( .5f, -.5f, -.5f), glm::vec3(.0f  , -1.0f, .0f)  , glm::vec3(.0f, .0f, .0f), glm::vec2(2.0f, .0f)},  // Lower right
};

// List of indices
GLuint indices[] = {
        // Front
        3 , 2 , 1,
        1 , 0 , 3,
        // Left
        7 , 6 , 5,
        5 , 4 , 7,
        // Back
        11, 10, 9,
        9 , 8 , 11,
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
Vertex lightVertices[] = {
        Vertex{glm::vec3(-0.1f, -0.1f, 0.1f)},
        Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f , -0.1f, -0.1f)},
        Vertex{glm::vec3(0.1f , -0.1f, 0.1f)},
        Vertex{glm::vec3(-0.1f, 0.1f , 0.1f)},
        Vertex{glm::vec3(-0.1f, 0.1f , -0.1f)},
        Vertex{glm::vec3(0.1f , 0.1f , -0.1f)},
        Vertex{glm::vec3(0.1f , 0.1f , 0.1)},
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

    // Texture
    Texture textures[] {
        Texture("res/textures/Brick_Wall_019_basecolor.jpg", "diffuse", 0, GL_RGB, GL_UNSIGNED_BYTE),
        Texture("res/textures/Brick_Wall_019_ambientOcclusion.jpg", "specular", 1, GL_RED, GL_UNSIGNED_BYTE),
    };

    /* --- Shader program --- */

    // Load shader program
    Shader shaderProgram("shaders/basic.vert", "shaders/basic.frag");

    // Create mesh
    std::vector<Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
    std::vector<GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
    std::vector<Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    Mesh cube(verts, ind, tex);

    /* --- Light shader program --- */

    // Load shader program
    Shader lightShader("shaders/light.vert", "shaders/light.frag");

    // Create mesh
    std::vector<Vertex> lightVerts(lightVertices, lightVertices + sizeof(lightVertices) / sizeof(Vertex));
    std::vector<GLuint> lightInd(lightIndices, lightIndices + sizeof(lightIndices) / sizeof(GLuint));
    Mesh light(lightVerts, lightInd, tex);

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
        cube.draw(shaderProgram, camera);

        /* --- Light program --- */
        light.draw(lightShader, camera);

        // Swap the display buffer
        glfwSwapBuffers(window);

        // Poll window event
        glfwPollEvents();
    }

    // Destroy everything
    shaderProgram.deleteFromGPU();
    lightShader.deleteFromGPU();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}