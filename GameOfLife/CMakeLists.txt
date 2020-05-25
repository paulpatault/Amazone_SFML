cmake_minimum_required(VERSION 3.14)
project(Jeu_de_la_vie)

set(CMAKE_CXX_STANDARD 17)

add_executable(Jeu_de_la_vie main.cpp Pool.cpp Pool.h)

find_package(SFML REQUIRED COMPONENTS graphics system)

target_link_libraries(Jeu_de_la_vie PRIVATE sfml-graphics sfml-system)

