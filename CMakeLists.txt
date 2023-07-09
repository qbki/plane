cmake_minimum_required(VERSION 3.15)
project(plane)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(GLM REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

file(GLOB SOURCES ./src/*)
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
