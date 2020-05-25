cmake_minimum_required(VERSION 3.14)
project(amazSFML)

set(CMAKE_CXX_STANDARD 17)

add_executable(amazSFML main.cpp Plateau.cpp Plateau.h)

find_package(SFML REQUIRED COMPONENTS graphics system)

target_link_libraries(amazSFML PRIVATE sfml-graphics sfml-system)

