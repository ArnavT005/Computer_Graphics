cmake_minimum_required(VERSION 3.0)

project(a2)
set(CMAKE_CXX_STANDARD 11)

find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)

add_library(viewer src/hw.cpp src/viewer.cpp src/mesh.cpp)
target_link_libraries(viewer GLEW::GLEW glm::glm OpenGL::GL SDL2::SDL2)

add_executable(example examples/example.cpp)
target_link_libraries(example viewer)

add_executable(square_mesh examples/square_mesh.cpp)
target_link_libraries(square_mesh viewer)

add_executable(sphere_mesh examples/sphere_mesh.cpp)
target_link_libraries(sphere_mesh viewer)

add_executable(bunny_mesh examples/bunny_mesh.cpp)
target_link_libraries(bunny_mesh viewer)

add_executable(cube_mesh examples/cube_mesh.cpp)
target_link_libraries(cube_mesh viewer)

add_executable(teapot_mesh examples/teapot_mesh.cpp)
target_link_libraries(teapot_mesh viewer)

add_executable(noisy_cube_mesh examples/noisy_cube_mesh.cpp)
target_link_libraries(noisy_cube_mesh viewer)