#include "openGL/Model.h"

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

    // Color of the light
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // Position of the light
    glm::vec3 lightPos = glm::vec3(.1f, 1.2, 0.0f);
    glm::mat4 lightModel = glm::mat4(1.0f);
    lightModel = glm::translate(lightModel, lightPos);

    // Put the data as Uniform
    shaderProgram.activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Allow the triangle to be rendered on only one side
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    // Create the camera
    Camera camera(width, height, glm::vec3(.0f, .0f, 2.0f));

    // Load the model
    Model model("res/models/stuff/ChantierStuff.gltf");

    while (!glfwWindowShouldClose(window)) {
        // Clear background
        glClearColor(.0f, .0f, .0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Read input to move the camera
        camera.inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Draw the model
        model.draw(shaderProgram, camera);

        // Swap the display buffer
        glfwSwapBuffers(window);

        // Poll window event
        glfwPollEvents();
    }

    // Destroy everything
    shaderProgram.deleteFromGPU();

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}