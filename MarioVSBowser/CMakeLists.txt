cmake_minimum_required(VERSION 3.14)
project(Plateforme)

set(CMAKE_CXX_STANDARD 17)

add_executable(Plateforme main.cpp)

find_package(SFML REQUIRED COMPONENTS graphics system)

target_link_libraries(Plateforme PRIVATE sfml-graphics sfml-system)

