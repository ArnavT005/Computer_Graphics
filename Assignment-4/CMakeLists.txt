cmake_minimum_required(VERSION 3.0)

project(a4)
set(CMAKE_CXX_STANDARD 11)

find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_library(a4 src/viewer.cpp src/hw.cpp src/mesh.cpp src/cloth.cpp src/rigidBody.cpp src/simulation.cpp)
target_link_libraries(a4 GLEW::GLEW glm::glm OpenGL::GL SDL2::SDL2 SDL2_image::SDL2_image)

add_executable(s1 examples/cloth_sim.cpp)
target_link_libraries(s1 a4)

add_executable(s2 examples/rigid_body_sim.cpp)
target_link_libraries(s2 a4)