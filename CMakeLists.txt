cmake_minimum_required(VERSION 3.15)
project(plane)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(SRC_MODELS_DIRECTORY ${CMAKE_SOURCE_DIR}/models)
set(SRC_SHADERS_DIRECTORY ${CMAKE_SOURCE_DIR}/shaders)
set(MODELS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/models)
set(SHADERS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/shaders)

find_package(GLM REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(TinyGLTF REQUIRED)

file(GLOB_RECURSE SOURCES ./src/*)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} TinyGLTF::TinyGLTF)

message(STATUS ${SRC_MODELS_DIRECTORY})
add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${SRC_MODELS_DIRECTORY} ${MODELS_DIRECTORY})

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${SRC_SHADERS_DIRECTORY} ${SHADERS_DIRECTORY})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
