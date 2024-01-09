cmake_minimum_required(VERSION 3.25)
project(untitled5)

set(CMAKE_CXX_STANDARD 17)

add_executable(untitled5 main.cpp MainWindow.cpp MainWindow.h Bollar.cpp Bollar.h)
find_package (SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories (${SFML_INCLUDE_DIRS})
target_link_libraries (untitled5 sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
