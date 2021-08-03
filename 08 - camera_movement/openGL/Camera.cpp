//
// Created by chokearth on 8/3/21.
//

#include "Camera.h"

// Init the parameter
Camera::Camera(int width, int height, glm::vec3 position) {
    Camera::width = width;
    Camera::height = height;
    Camera::position = position;
}

// Calculate the view and projection matrix and put them in a uniform
void Camera::matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform) {
    // Create the view and projection matrix
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Make the camera look in the direction corresponding to the orientation
    view = glm::lookAt(position, position + orientation, up);
    // Generate the projection matrix
    projection = glm::perspective(glm::radians(FOVdeg), (float) (width / height), nearPlane, farPlane);

    // Put the result as a uniform
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

// Manage the input to move the camera
void Camera::inputs(GLFWwindow *window) {
    // Manage movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) position += speed * glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z));
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) position += speed * -glm::normalize(glm::cross(orientation, up));
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) position += speed * -glm::normalize(glm::vec3(orientation.x, 0.0f, orientation.z));
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) position += speed * glm::normalize(glm::cross(orientation, up));
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) position += speed * up;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) position += speed * -up;

    // Mouse movement
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // Hide cursor
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        // if not first tick with mouse locked
        if (!firstClick) {

            // Get mouse position
            double mouseX;
            double mouseY;
            glfwGetCursorPos(window, &mouseX, &mouseY);

            // Calculate the rotation with the position of the mouse
            float rotX = sensitivity * (float) (mouseY - (height / 2)) / height;
            float rotY = sensitivity * (float) (mouseX - (width / 2)) / width;

            // Create the new orientation with the X rotation
            glm::vec3 newOrientation = glm::rotate(orientation, glm::radians(-rotX),
                                                   glm::normalize(glm::cross(orientation, up)));

            // Check that it is not too low or too high to avoid getting inverted
            if (!((glm::angle(newOrientation, up) <= glm::radians(5.0f)) or
            (glm::angle(newOrientation, -up) <= glm::radians(5.0f))))
                orientation = newOrientation;

            // Rotate on the Y axis
            orientation = glm::rotate(orientation, glm::radians(-rotY), up);
        }
        firstClick = false;

        // Reset the position of the cursor to the center of the window
        glfwSetCursorPos(window, (width / 2), (height / 2));

    } else if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE) {
        // Unlock the mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        firstClick = true;
    }
}