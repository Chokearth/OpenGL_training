cmake_minimum_required(VERSION 3.20)
project(OpenGLTraining)

set(CMAKE_CXX_STANDARD 23)
set(TARGET SimpleTriangle)

find_package(glfw3 3.3 REQUIRED)

include_directories(../Libraries/include)

add_executable(${TARGET} main.cpp ../Libraries/lib/glad.c)
target_link_libraries(${TARGET} glfw)

IF (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)

        message(STATUS "Add resources")
        add_custom_command(
                TARGET ${TARGET} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${TARGET}>/shaders/
        )
endif()

