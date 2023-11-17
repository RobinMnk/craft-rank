cmake_minimum_required(VERSION 3.21)
project(CraftRank)

set(CMAKE_CXX_STANDARD 20)

add_executable(CraftRank main.cpp)
target_compile_options(CraftRank PUBLIC -Wall -Wextra)