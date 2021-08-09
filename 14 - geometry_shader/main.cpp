#include "openGL/Model.h"

const unsigned int width = 800;
const unsigned int height = 800;

float rectangleVertices[] = {
        1.0f , -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f , 0.0f, 1.0f,

        1.0f , 1.0f , 1.0f, 1.0f,
        1.0f , -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f , 0.0f, 1.0f,
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
    Shader shaderProgram("shaders/basic.vert", "shaders/basic.frag", "shaders/basic.geom");
    Shader normalsProgram("shaders/basic.vert", "shaders/normals.frag", "shaders/normals.geom");

    // Color of the light
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // Position of the light
    glm::vec3 lightPos = glm::vec3(.1f, 1.2, 0.0f);

    // Put the data as Uniform
    shaderProgram.activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z,
                lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

    // Allow the triangle to be rendered on only one side
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);

    // Create the camera
    Camera camera(width, height, glm::vec3(4.8f, 5.0f, 18.0f));

    // Load the model
    Model model("res/models/stuff/ChantierStuff.gltf");

    // Time variable
    double prevTime = .0;
    double crntTime = .0;
    double timeDiff;
    unsigned int counter = 0;

    // Disable VSync
//    glfwSwapInterval(0);

    while (!glfwWindowShouldClose(window)) {
        // Get the time
        crntTime = glfwGetTime();
        timeDiff = crntTime - prevTime;
        counter++;
        if (timeDiff > 1.0 / 30.0) {
            // Create new Title
            std::string FPS = std::to_string((1.0 / timeDiff) * counter);
            std::string ms = std::to_string((timeDiff / counter) * 1000);
            std::string newTitle = "OpenGL - " + FPS + "FPS / " + ms + "ms";
            glfwSetWindowTitle(window, newTitle.c_str());

            // Reset variable
            prevTime = crntTime;
            counter = 0;
        }

        // Clear background
        glClearColor(.25f, .25f, .30f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Read input to move the camera
        camera.inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Draw the model
        model.draw(shaderProgram, camera);
        model.draw(normalsProgram, camera);

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