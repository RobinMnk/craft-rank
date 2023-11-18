cmake_minimum_required(VERSION 3.21)
project(CraftRank)

set(CMAKE_CXX_STANDARD 20)
# Include the 'src' subdirectories
add_subdirectory(src)

add_executable(CraftRank src/main.cpp)

# Link libraries
target_link_libraries(CraftRank db_reader rank_algo)
target_compile_options(CraftRank PUBLIC -Wall -Wextra)
