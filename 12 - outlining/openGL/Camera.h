//
// Created by chokearth on 8/3/21.
//

#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "Shader.h"

class Camera {
public:
    // Position of the camera
    glm::vec3 position;
    // orientation of the camera
    glm::vec3 orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    // vector to manage the row axis
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    // Matrix of the camera ( projection * view )
    glm::mat4 cameraMatrix = glm::mat4(1.0f);

    // Allow to ignore the first click for the camera movement
    bool firstClick = true;

    // width and height of the camera screen
    int width;
    int height;

    // movement speed and mouse sensitivity
    float speed = 0.1f;
    float sensitivity = 100.0f;

    // Init the parameter
    Camera(int width, int height, glm::vec3 position);

    // Calculate the view and projection matrix
    void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
    // Put the camera matrix in a uniform
    void matrix(Shader &shader, const char *uniform);

    // Manage the input to move the camera
    void inputs(GLFWwindow *window);
};