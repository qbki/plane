cmake_minimum_required(VERSION 3.31.6)
include(${CMAKE_SOURCE_DIR}/configs/cmake/utils.cmake)
include(${CMAKE_SOURCE_DIR}/configs/cmake/license.cmake)

set(CMAKE_TOOLCHAIN_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/tools/vcpkg/scripts/buildsystems/vcpkg.cmake"
    CACHE STRING "Vcpkg toolchain file")

project(plane VERSION 0.1.7)

function(setup_link_libraries target_name)
  target_link_libraries(${target_name} PRIVATE EnTT::EnTT)
  target_link_libraries(${target_name} PRIVATE Microsoft.GSL::GSL)
  target_link_libraries(${target_name} PRIVATE glm::glm-header-only)
  target_link_libraries(${target_name} PRIVATE PkgConfig::thorvg)

  target_link_libraries(${target_name} PRIVATE nlohmann_json::nlohmann_json)
  set(nlohmann-json_IMPLICIT_CONVERSIONS OFF)

  target_include_directories(${target_name} PRIVATE ${TINYGLTF_INCLUDE_DIRS})

  if(DEFINED EMSCRIPTEN)
    target_include_directories(${target_name} PUBLIC "${EMSCRIPTEN_SYSROOT}/include")
    target_link_libraries(${target_name} PRIVATE GL GLEW SDL2)
  else()
    target_link_libraries(${target_name} PRIVATE GLEW::GLEW)
    target_link_libraries(${target_name} PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
      $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>)
    target_link_libraries(${target_name} PRIVATE
      $<IF:$<TARGET_EXISTS:SDL2_mixer::SDL2_mixer>,SDL2_mixer::SDL2_mixer,SDL2_mixer::SDL2_mixer-static>)
  endif()

  if(${BUILD_TESTS})
    target_link_libraries(${target_name} PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)
  endif()
endfunction()

set(JSON_ImplicitConversions, OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(SRC_SHADERS_DIRECTORY ${CMAKE_SOURCE_DIR}/shaders)
set(SRC_ASSETS_DIRECTORY ${CMAKE_SOURCE_DIR}/assets)
set(SHADERS_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders)
set(ASSETS_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets)
set(WEB_SHELL_BUILD_DIRECTORY ${CMAKE_SOURCE_DIR}/build/web-shell)

set(THIRD_PARTY_LICENSES_FILE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/THIRD_PARTY_LICENSES)

option(BUILD_TESTS "Turn this ON to compile tests" OFF)
option(BUILD_EXECUTABLE "Turn this ON to compile an executable file" OFF)

find_package(EnTT CONFIG REQUIRED)
find_package(Microsoft.GSL CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_path(TINYGLTF_INCLUDE_DIRS "tiny_gltf.h")
find_package(Catch2 CONFIG REQUIRED)

# <thorvg>
find_package(PkgConfig)
pkg_check_modules(thorvg REQUIRED IMPORTED_TARGET thorvg)
# </thorvg>

if(NOT DEFINED EMSCRIPTEN)
  find_package(GLEW REQUIRED)
  find_package(SDL2 CONFIG REQUIRED)
  find_package(SDL2_mixer CONFIG REQUIRED)
endif()

include_directories(${CMAKE_SOURCE_DIR})
include_directories(${catch2_INCLUDE_DIRS_RELEASE})

add_compile_flag("-Wall")
add_compile_flag("-Wextra")
add_compile_flag("-Wpedantic")
add_compile_flag("-pedantic-errors")
add_compile_flag("-fno-exceptions")
if(DEFINED EMSCRIPTEN)
  add_compile_flag("-O1")
  add_linker_flag("-sUSE_SDL=2")
  add_linker_flag("-sUSE_SDL_MIXER=2")
  add_linker_flag("-sWASM=1")
  add_linker_flag("-sEXPORT_NAME=moduleFactory")
  add_linker_flag("-sMODULARIZE")
  add_linker_flag("-sEXPORT_ES6=1")
  # Removes node.js support from the wasm build. Without this option
  # webpack tries import node.js modules that can't be found or vite creates
  # unnecessary chunks with modules from node.js.
  add_linker_flag("-sENVIRONMENT=web")
  # It allows to use an arbitrary id for canvas element in html. In other
  # case Emscripten uses hardcoded name ("canvas").
  add_linker_flag("-sDISABLE_DEPRECATED_FIND_EVENT_TARGET_BEHAVIOR=0")
  add_linker_flag("-lidbfs.js")
  add_linker_flag("-sASYNCIFY")
  add_linker_flag("-sALLOW_MEMORY_GROWTH")
  add_linker_flag("-sFULL_ES3")
  add_linker_flag("-sMAX_WEBGL_VERSION=2")
  add_linker_flag("-sMIN_WEBGL_VERSION=2")
  #DEBUG
  #add_linker_flag("-g3")
  #add_linker_flag("-gsource-map")
endif()

file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE SOURCES_MODULES src/*.cppm)
set(TEST_SOURCES ${SOURCES})
set(MODULES_SOURCES src/control.cpp)
list(FILTER SOURCES EXCLUDE REGEX ".tests.cpp$")
list(FILTER TEST_SOURCES EXCLUDE REGEX "main.cpp$")

if(BUILD_EXECUTABLE)
  add_executable(${PROJECT_NAME} ${SOURCES})
  target_sources(${PROJECT_NAME}
    PUBLIC
      FILE_SET cpp_modules
      TYPE CXX_MODULES FILES ${SOURCES_MODULES})
  target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
  setup_link_libraries(${PROJECT_NAME})
  if(DEFINED EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "index")
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".js")
    # <> It helps generate the right compile_commangs.json file for clangd
    set(ARGS --cflags -sUSE_SDL=2 -sUSE_SDL_MIXER=2)
    execute_process(COMMAND ${CMAKE_CXX_COMPILER} ${ARGS}
                    OUTPUT_VARIABLE EMSCRIPTEN_FLAGS)
    set_target_properties(${PROJECT_NAME} PROPERTIES COMPILE_FLAGS "${EMSCRIPTEN_FLAGS}")
    # </>
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
  generate_licenses_file()
endif()

if(BUILD_TESTS)
  enable_testing()
  set(TESTS_TARGET tests)
  add_executable(${TESTS_TARGET} ${TEST_SOURCES})
  set_target_properties(
    tests
    PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tests)
  add_test(NAME ${TESTS_TARGET} COMMAND "tests/${TESTS_TARGET}")
  setup_link_libraries(${TESTS_TARGET})
endif()

if(BUILD_EXECUTABLE)
  install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION .)
  install(FILES ${THIRD_PARTY_LICENSES_FILE} DESTINATION .)
  install(DIRECTORY ${SRC_SHADERS_DIRECTORY} DESTINATION .)
  if(DEFINED EMSCRIPTEN)
    install(DIRECTORY ${WEB_SHELL_BUILD_DIRECTORY}/ DESTINATION .)
  else()
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION .)
  endif()
  include(${CMAKE_SOURCE_DIR}/configs/cmake/pack.cmake)
endif()
