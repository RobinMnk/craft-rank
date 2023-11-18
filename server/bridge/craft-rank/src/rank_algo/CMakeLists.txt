# Define the library target
add_library(rank_algo craft_rank.cpp craft_rank.h core.h core.cpp)


target_compile_options(rank_algo PUBLIC -O3)

# Specify any include directories if needed
target_include_directories(rank_algo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})