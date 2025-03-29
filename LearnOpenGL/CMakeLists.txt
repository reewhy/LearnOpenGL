cmake_minimum_required(VERSION 3.10)
project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenGL
find_package(OpenGL REQUIRED)

# GLFW paths (Adjust based on your system)
set(GLFW_INCLUDE_DIR "${PROJECT_SOURCE_DIR}/libs/glfw/include")
set(GLFW_LIB_DIR "${PROJECT_SOURCE_DIR}/libs/glfw/lib-mingw-w64")

include_directories(${GLFW_INCLUDE_DIR})
link_directories(${GLFW_LIB_DIR})

# Add GLM
find_package(glm CONFIG REQUIRED)

# Add GLAD
add_library(glad "${PROJECT_SOURCE_DIR}/libs/glad/src/glad.c")
target_include_directories(glad PUBLIC "${PROJECT_SOURCE_DIR}/libs/glad/include")

# Add the executable
add_executable(LearnOpenGL main.cpp Shader.cpp
        Texture.h)

# Link libraries
target_link_libraries(LearnOpenGL PRIVATE glad OpenGL::GL glfw3 glm::glm)
