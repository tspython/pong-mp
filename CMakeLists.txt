cmake_minimum_required(VERSION 3.10)
project(Pong)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add source files
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

find_package(glm REQUIRED)
include_directories(${GLM_INCLUDE_DIRS})

# Find GLFW
find_package(glfw3 REQUIRED)
add_library(GLAD lib/glad/src/glad.c)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/lib/glad/include)
include_directories(${glfw3_INCLUDE_DIRS})


add_executable(Pong ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} glfw)
target_link_libraries(${PROJECT_NAME} GLAD)
