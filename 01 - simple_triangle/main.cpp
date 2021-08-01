#include <iostream>
#include <fstream>
#include <cstring>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <math.h>

void loadShader(char ** shaderBuffer, char *path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open shader file \"" << path << "\"" << std::endl;
        return;
    }

    std::string ret = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();

    (*shaderBuffer) = new char[ret.length() + 1];
    std::strcpy((*shaderBuffer), ret.c_str());
}

void compileShader(GLuint shaderID, char *source) {
    glShaderSource(shaderID, 1, &source, NULL);
    glCompileShader(shaderID);

    GLint result = GL_FALSE;
    int infoLogLength = 1024;
    char shaderErrorMessage[1024] = {0};

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

    glGetShaderInfoLog(shaderID, infoLogLength, NULL, shaderErrorMessage);
    if (std::strlen(shaderErrorMessage) != 0)
        std::cerr << shaderErrorMessage << std::endl;
}

GLfloat vertices[] = {
        -.5f, -.5f * float(sqrt(3)) / 3, .0f,
        .5f, -.5f * float(sqrt(3)) / 3, .0f,
        -.0f, .5f * float(sqrt(3)) * 2 / 3, .0f
};

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
    GLFWwindow* window = glfwCreateWindow(800, 800, "Simple triangle", NULL, NULL);
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
    GLuint shaderProgram = glCreateProgram();
    {
        char * vertexShaderSource;
        char * fragmentShaderSource;

        loadShader(&vertexShaderSource, "./shaders/basic.vert");
        GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
        compileShader(vertexShader, vertexShaderSource);

        loadShader(&fragmentShaderSource, "./shaders/basic.frag");
        GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        compileShader(fragmentShader, fragmentShaderSource);

        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
    }

    // Create reference containers for the Vertex Array Object, the Vertex Buffer Object and the Element Buffer Object
    GLuint VAO, VBO, EBO;

    // Generate the VAO, VBO and EBO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the VAO as the current Vertex Array Object
    glBindVertexArray(VAO);

    // Bind the VBO as the current Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // Insert the vertices into the VBO
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Bind the EBO as the current Element Buffer Object
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // Insert the vertices into the EBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configure the Vertex Attribute so that OpenGL knows how to read the VBO
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);
    // Enable the Vertex Attribute
    glEnableVertexAttribArray(0);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Change background color
    glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);

    while (!glfwWindowShouldClose(window)) {
        glClearColor(.0f, .0f, .0f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // Destroy everything
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
