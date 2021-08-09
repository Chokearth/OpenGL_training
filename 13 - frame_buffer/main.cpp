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
    Shader shaderProgram("shaders/basic.vert", "shaders/basic.frag");
    Shader framebufferProgram("shaders/framebuffer.vert", "shaders/framebuffer.frag");

    // Color of the light
    glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    // Position of the light
    glm::vec3 lightPos = glm::vec3(.1f, 1.2, 0.0f);

    // Put the data as Uniform
    shaderProgram.activate();
    glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z,
                lightColor.w);
    glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
    framebufferProgram.activate();
    glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);

    // Allow the triangle to be rendered on only one side
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);
    glFrontFace(GL_CCW);
    glEnable(GL_DEPTH_TEST);

    // Create the camera
    Camera camera(width, height, glm::vec3(4.8f, 5.0f, 18.0f));

    // Load the model
    Model model("res/models/stuff/ChantierStuff.gltf");

    unsigned int rectVAO, rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindVertexArray(rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float)));

    // Time variable
    double prevTime = .0;
    double crntTime = .0;
    double timeDiff;
    unsigned int counter = 0;

    // Disable VSync
//    glfwSwapInterval(0);

    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create the texture
    unsigned int framebufferTexture;
    glGenTextures(1, &framebufferTexture);
    glBindTexture(GL_TEXTURE_2D, framebufferTexture);
    // Set parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture, 0);

    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cerr << "Framebuffer error: " << fboStatus << std::endl;

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

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        // Clear background
        glClearColor(.25f, .25f, .30f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        // Read input to move the camera
        camera.inputs(window);
        camera.updateMatrix(45.0f, 0.1f, 100.0f);

        // Draw the model
        model.draw(shaderProgram, camera);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        framebufferProgram.activate();
        glBindVertexArray(rectVAO);
        glDisable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, framebufferTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        // Swap the display buffer
        glfwSwapBuffers(window);

        // Poll window event
        glfwPollEvents();
    }

    // Destroy everything
    shaderProgram.deleteFromGPU();
    glDeleteFramebuffers(1, &FBO);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}