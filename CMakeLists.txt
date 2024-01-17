cmake_minimum_required(VERSION 3.15)

if(DEFINED EMSCRIPTEN)
  project(index)
  set(CMAKE_EXECUTABLE_SUFFIX ".html")
else()
  project(plane)
endif()

function(print_variables)
  get_cmake_property(names VARIABLES)
  list (SORT names)
  foreach (name ${names})
      message(STATUS "${name}=${${name}}")
  endforeach()
endfunction()

function(add_flag flag)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
endfunction()

function(add_exe_flag flag)
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${flag}" PARENT_SCOPE)
endfunction()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(SRC_SHADERS_DIRECTORY ${CMAKE_SOURCE_DIR}/shaders)
set(SRC_ASSETS_DIRECTORY ${CMAKE_SOURCE_DIR}/assets)
set(SHADERS_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
set(ASSETS_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)

set(JSON_ImplicitConversions, OFF)
include_directories(${CMAKE_SOURCE_DIR})

find_package(EnTT REQUIRED)
find_package(GLM REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(TinyGLTF REQUIRED)
find_package(nlohmann_json REQUIRED)

add_flag("-Wall")
add_flag("-Wextra")
add_flag("-Wpedantic")
add_flag("-pedantic-errors")
if(DEFINED EMSCRIPTEN)
  add_flag("-fexceptions")
  add_flag("-std=c++20")
  add_flag("-sUSE_SDL=2")
  add_exe_flag("--preload-file ${SRC_SHADERS_DIRECTORY}/@/shaders/")
  add_exe_flag("--shell-file ${PROJECT_SOURCE_DIR}/src/shell.html")
  add_exe_flag("-fexceptions")
  add_exe_flag("-sASYNCIFY")
  add_exe_flag("-sALLOW_MEMORY_GROWTH")
  add_exe_flag("-sEXPORT_EXCEPTION_HANDLING_HELPERS")
  add_exe_flag("-sFULL_ES3")
  add_exe_flag("-sMAX_WEBGL_VERSION=2")
  add_exe_flag("-sMIN_WEBGL_VERSION=2")
else()
  find_package(GLEW REQUIRED)
  find_package(SDL2 REQUIRED)
endif()


file(GLOB_RECURSE SOURCES ./src/*)
add_executable(${PROJECT_NAME} ${SOURCES})

if(DEFINED EMSCRIPTEN)
  target_include_directories(${PROJECT_NAME} PUBLIC "${EMSCRIPTEN_SYSROOT}/include")
  target_link_libraries(${PROJECT_NAME} GL GLEW SDL2)
else()
  target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
  target_link_libraries(${PROJECT_NAME} SDL2::SDL2main)
endif()

target_link_libraries(${PROJECT_NAME} EnTT::EnTT)
target_link_libraries(${PROJECT_NAME} Microsoft.GSL::GSL)
target_link_libraries(${PROJECT_NAME} TinyGLTF::TinyGLTF)
target_link_libraries(${PROJECT_NAME} glm::glm)
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json)

if(DEFINED EMSCRIPTEN)
else()
  add_custom_command(
    TARGET ${PROJECT_NAME} PRE_LINK
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${SRC_SHADERS_DIRECTORY} ${SHADERS_DIRECTORY})
endif()

add_custom_command(
  TARGET ${PROJECT_NAME} PRE_LINK
  COMMAND ${CMAKE_COMMAND} -E create_symlink ${SRC_ASSETS_DIRECTORY} ${ASSETS_DIRECTORY})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
