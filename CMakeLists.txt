cmake_minimum_required(VERSION 3.20)
project(Program)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(Program
    src/Skeleton.cpp
    src/framework.cpp
    src/Camera.cpp
    src/GameObject.cpp
    src/Geometry.cpp
    src/Scene.cpp
    src/Shader.cpp
    src/Tank.cpp
    src/main.cpp
    external/Glad/src/glad.c
)

target_include_directories(Program PRIVATE
    include/
    external/Glad/include
    external/glm/include
)

target_link_libraries(Program PRIVATE
    OpenGL::GL
    glfw
)