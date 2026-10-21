cmake_minimum_required(VERSION 3.16)
project(MyGame)

set(CMAKE_CXX_STANDARD 17)

# Tell CMake to use the right SFML path from MSYS2
set(CMAKE_PREFIX_PATH "C:/msys64/ucrt64")

# Use Ninja as the generator
set(CMAKE_GENERATOR Ninja)

# Find SFML
find_package(SFML 2 REQUIRED graphics window system)

add_executable(MyGame src/game.cpp)
target_link_libraries(MyGame sfml-graphics sfml-window sfml-system)