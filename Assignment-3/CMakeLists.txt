cmake_minimum_required(VERSION 3.0)

project(a3)
set(CMAKE_CXX_STANDARD 11)

find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_library(a3 src/rayTracer.cpp src/object.cpp src/sphere.cpp src/plane.cpp src/box.cpp src/pointSource.cpp)
target_link_libraries(a3 GLEW::GLEW glm::glm OpenGL::GL SDL2::SDL2 SDL2_image::SDL2_image)

add_executable(s1 examples/s1.cpp)
target_link_libraries(s1 a3)

add_executable(s2 examples/s2.cpp)
target_link_libraries(s2 a3)

add_executable(s3 examples/s3.cpp)
target_link_libraries(s3 a3)

add_executable(s4 examples/s4.cpp)
target_link_libraries(s4 a3)

add_executable(s5 examples/s5.cpp)
target_link_libraries(s5 a3)

add_executable(s6 examples/s6.cpp)
target_link_libraries(s6 a3)

add_executable(s6_diffuse examples/s6_diffuse.cpp)
target_link_libraries(s6_diffuse a3)

add_executable(s7 examples/s7.cpp)
target_link_libraries(s7 a3)

add_executable(s8 examples/s8.cpp)
target_link_libraries(s8 a3)