cmake_minimum_required(VERSION 3.27)
include(${CMAKE_SOURCE_DIR}/configs/cmake/utils.cmake)
include(${CMAKE_SOURCE_DIR}/configs/cmake/license.cmake)

project(plane VERSION 0.1.5)

function(setup_link_libraries target_name)
  target_link_libraries(${target_name} EnTT::EnTT)
  target_link_libraries(${target_name} Microsoft.GSL::GSL)
  target_link_libraries(${target_name} TinyGLTF::TinyGLTF)
  target_link_libraries(${target_name} glm::glm)
  target_link_libraries(${target_name} nlohmann_json::nlohmann_json)
  if(DEFINED EMSCRIPTEN)
    target_include_directories(${target_name} PUBLIC "${EMSCRIPTEN_SYSROOT}/include")
    target_link_libraries(${target_name} GL GLEW SDL2)
  else()
    target_link_libraries(${target_name} GLEW::GLEW)
    target_link_libraries(${target_name} SDL2::SDL2main)
  endif()
endfunction()

function(generate_licenses_file)
  set(licenses "")
  string(APPEND licenses
    "This file was generated semiautomatically and it\n"
    "might contain errors or lack information. Please\n"
    "check the official sites of these libraries to get\n"
    "current legal information.\n\n"
    "This software is distributed with further described\n"
    "libraries but is not limited. If your library\n"
    "or/and license is missing in the file, please\n"
    "address it to the maintainers of Plane Game Engine.\n\n")
  append_license(licenses "EnTT" "EnTT" "entt")
  append_license(licenses "Microsoft.GSL" "Microsoft.GSL" "ms-gsl")
  append_license(licenses "glm" "glm" "glm")
  append_license(licenses "nlohmann/json" "nlohmann_json" "nlohmann_json")
  append_license(licenses "TinyGLTF" "TinyGLTF" "tinygltf")
  if(DEFINED EMSCRIPTEN)
    # Emscripten is a tool dependency and it doesn't have
    # generated constants like other dependencies.
    append_license_title(licenses "Emscripten" ${EMSCRIPTEN_VERSION})
    append_license_file(licenses "${EMSCRIPTEN_ROOT_PATH}/../../..")
  else()
    append_license(licenses "SDL2" "SDL2" "sdl")
    append_license(licenses "GLEW" "glew" "glew")
  endif()
  file(WRITE ${THIRD_PARTY_LICENSES_FILE} ${licenses})
endfunction()

set(JSON_ImplicitConversions, OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)

set(SRC_SHADERS_DIRECTORY ${CMAKE_SOURCE_DIR}/shaders)
set(SRC_ASSETS_DIRECTORY ${CMAKE_SOURCE_DIR}/assets)
set(SHADERS_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
set(ASSETS_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
set(WEB_SHELL_BUILD_DIRECTORY ${CMAKE_SOURCE_DIR}/build/web-shell)

set(THIRD_PARTY_LICENSES_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/THIRD_PARTY_LICENSES)

option(BUILD_TESTS "Turn this ON to compile tests" OFF)
option(BUILD_EXECUTABLE "Turn this ON to compile an executable file" OFF)

find_package(Catch2 3 REQUIRED)
find_package(EnTT REQUIRED)
find_package(GLM REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(TinyGLTF REQUIRED)
find_package(nlohmann_json REQUIRED)
if(NOT DEFINED EMSCRIPTEN)
  find_package(GLEW REQUIRED)
  find_package(SDL2 REQUIRED)
endif()

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${catch2_INCLUDE_DIRS_RELEASE})

add_flag("-Wall")
add_flag("-Wextra")
add_flag("-Wpedantic")
add_flag("-pedantic-errors")
if(DEFINED EMSCRIPTEN)
  add_flag("-fexceptions")
  add_flag("-std=c++20")
  add_flag("-sUSE_SDL=2")
  add_exe_flag("-sEXPORT_NAME=moduleFactory")
  add_exe_flag("-sMODULARIZE")
  add_exe_flag("-sEXPORT_ES6=1")
  # Removes node.js support from the wasm build. Without this option
  # webpack tries import node.js modules that can't be found or vite creates
  # unnecessary chunks with modules from node.js.
  add_exe_flag("-sENVIRONMENT=web")
  # It allows to use an arbitrary id for canvas element in html. In other
  # case Emscripten uses hardcoded name ("canvas").
  add_exe_flag("-sDISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0")
  add_exe_flag("--preload-file ${SRC_SHADERS_DIRECTORY}/@/shaders/")
  add_exe_flag("-fexceptions")
  add_exe_flag("-sASYNCIFY")
  add_exe_flag("-sALLOW_MEMORY_GROWTH")
  add_exe_flag("-sEXPORT_EXCEPTION_HANDLING_HELPERS")
  add_exe_flag("-sFULL_ES3")
  add_exe_flag("-sMAX_WEBGL_VERSION=2")
  add_exe_flag("-sMIN_WEBGL_VERSION=2")
endif()

file(GLOB_RECURSE SOURCES src/*)
set(TEST_SOURCES ${SOURCES})
list(FILTER SOURCES EXCLUDE REGEX ".tests.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX "main.cpp$")

if(BUILD_EXECUTABLE)
  add_executable(${PROJECT_NAME} ${SOURCES})
  setup_link_libraries(${PROJECT_NAME})
  if(DEFINED EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".js")
  else()
    add_custom_command(
      TARGET ${PROJECT_NAME} PRE_LINK
      COMMAND ${CMAKE_COMMAND} -E
      create_symlink ${SRC_SHADERS_DIRECTORY} ${SHADERS_DIRECTORY})
    add_custom_command(
      TARGET ${PROJECT_NAME} PRE_LINK
      COMMAND ${CMAKE_COMMAND} -E
      create_symlink ${SRC_ASSETS_DIRECTORY} ${ASSETS_DIRECTORY})
  endif()
  if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
    generate_licenses_file()
  endif()
endif()

if(BUILD_TESTS)
  enable_testing()
  set(TESTS_TARGET tests)
  add_executable(${TESTS_TARGET} ${TEST_SOURCES})
  set_target_properties(
    tests
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests)
  if(DEFINED EMSCRIPTEN)
    add_test(NAME ${TESTS_TARGET} COMMAND node "tests/${TESTS_TARGET}.js")
  else()
    add_test(NAME ${TESTS_TARGET} COMMAND tests/${TESTS_TARGET})
  endif()
  setup_link_libraries(${TESTS_TARGET})
  target_link_libraries(${TESTS_TARGET} Catch2::Catch2WithMain)
endif()

if(BUILD_EXECUTABLE)
  install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION .)
  install(FILES ${THIRD_PARTY_LICENSES_FILE} DESTINATION .)
  if(DEFINED EMSCRIPTEN)
    install(DIRECTORY ${WEB_SHELL_BUILD_DIRECTORY}/ DESTINATION .)
  else()
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
    install(DIRECTORY ${SRC_SHADERS_DIRECTORY} DESTINATION .)
  endif()
  include(${CMAKE_SOURCE_DIR}/configs/cmake/pack.cmake)
endif()
