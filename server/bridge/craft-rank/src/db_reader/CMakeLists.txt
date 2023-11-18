# Define the library target
add_library(db_reader database_reader.cpp database_reader.h)

# Specify any include directories if needed
target_include_directories(db_reader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})