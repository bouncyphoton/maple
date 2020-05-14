cmake_minimum_required(VERSION 3.15)
project(maple)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

find_package(SDL2 REQUIRED)

add_executable(maple third-party/gl3w/gl3w.c src/main.cpp src/core.cpp src/core.h src/platform.cpp src/platform.h src/game_state.h src/types.h src/constants.h src/renderer.cpp src/renderer.h src/shader.cpp src/shader.h src/utils.h)
target_link_libraries(maple ${SDL2_LIBRARY})
target_include_directories(maple PUBLIC ${SDL2_INCLUDE_DIR} third-party/gl3w/include)

# Don't forget the SDL2 DLL :)
