cmake_minimum_required(VERSION 3.20)
project(Program)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)

FetchContent_Declare(json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(json)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(Program
    ${SOURCES}
    external/Glad/src/glad.c
    external/picoPNG/picoPNG.cpp
)

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR NOT CMAKE_BUILD_TYPE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        
        target_compile_options(Program PRIVATE -fsanitize=address -fno-omit-frame-pointer)
        target_link_options(Program PRIVATE -fsanitize=address)
    endif()
endif()

target_include_directories(Program PRIVATE
    src
    include/
    external/Glad/include
    external/glm/include
    external/picoPNG 
)

target_link_libraries(Program PRIVATE
    OpenGL::GL
    glfw
    nlohmann_json::nlohmann_json
)