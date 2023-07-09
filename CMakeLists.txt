cmake_minimum_required(VERSION 3.15)
project(plane)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(MODELS_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/models/)

find_package(GLM REQUIRED)
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(TinyGLTF REQUIRED)

file(GLOB SOURCES ./src/*)
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} SDL2::SDL2main)
target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} TinyGLTF::TinyGLTF)

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory ${MODELS_DIRECTORY})

file(GLOB GLB_FILES ./models/*.glb)
foreach(file IN LISTS GLB_FILES)
  add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${file} ${MODELS_DIRECTORY}
    COMMENT "Copying: ${file}")
endforeach()

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 17)
