cmake_minimum_required(VERSION 3.20)
project(OpenGLTraining)

set(CMAKE_CXX_STANDARD 23)
set(TARGET computeShader)

find_package(glfw3 3.3 REQUIRED)

include_directories(../Libraries/include)

add_subdirectory(graphics)

add_executable(${TARGET} main.cpp ../Libraries/lib/glad.c ../Libraries/lib/stb.cpp ${SOURCES})
target_link_libraries(${TARGET} glfw)

ADD_CUSTOM_TARGET(
        copy_shader_files
        ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/shaders $<TARGET_FILE_DIR:${TARGET}>/shaders
)
ADD_DEPENDENCIES(${TARGET} copy_shader_files)

