# Define the library target
add_library(rank_algo craft_rank.cpp craft_rank.h)

# Specify any include directories if needed
target_include_directories(rank_algo PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})