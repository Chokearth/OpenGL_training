cmake_minimum_required(VERSION 3.20)
project(OpenGLTraining)

set(CMAKE_CXX_STANDARD 23)
set(TARGET simpleTexture)

find_package(glfw3 3.3 REQUIRED)

include_directories(../Libraries/include)

add_subdirectory(graphics)

add_executable(${TARGET} main.cpp ../Libraries/lib/glad.c ../Libraries/lib/stb.cpp ${SOURCES})
target_link_libraries(${TARGET} glfw)

IF (NOT CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_CURRENT_BINARY_DIR)

        message(STATUS "Add resources")
        set(RESOURCES shaders res)

        foreach(item IN LISTS RESOURCES)
                add_custom_command(
                        TARGET ${TARGET} POST_BUILD
                        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/${item} $<TARGET_FILE_DIR:${TARGET}>/${item}
                )
        endforeach()
endif()

