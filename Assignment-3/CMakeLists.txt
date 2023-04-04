cmake_minimum_required(VERSION 3.0)

project(a3)
set(CMAKE_CXX_STANDARD 11)

find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

add_library(a3 src/rayTracer.cpp src/object.cpp src/sphere.cpp src/plane.cpp src/box.cpp src/pointSource.cpp)
target_link_libraries(a3 GLEW::GLEW glm::glm OpenGL::GL SDL2::SDL2)

add_executable(spheres examples/spheres.cpp)
target_link_libraries(spheres a3)

add_executable(sphere_plane examples/sphere_plane.cpp)
target_link_libraries(sphere_plane a3)