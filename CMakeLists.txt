cmake_minimum_required(VERSION 3.15)
project(maple)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)

add_executable(maple third-party/gl3w/gl3w.c src/main.cpp src/core.cpp src/core.h src/platform.cpp src/platform.h src/game_state.h src/types.h src/constants.h src/renderer.cpp src/renderer.h src/shader.cpp src/shader.h src/utils.h src/model.h src/mesh.cpp src/mesh.h src/vertex.h src/camera.h src/material.h src/scene.cpp src/scene.h)
target_link_libraries(maple ${SDL2_LIBRARY})
target_include_directories(maple PUBLIC ${SDL2_INCLUDE_DIR} third-party/gl3w/include third-party/glm third-party/tinyobjloader)

# Don't forget the SDL2 DLL :)
