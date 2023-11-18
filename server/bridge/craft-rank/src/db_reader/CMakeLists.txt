# Locate the SQLite3 package
find_package(SQLite3 REQUIRED)

# Define the library target
add_library(db_reader database_reader.cpp database_reader.h)

target_link_libraries(db_reader SQLite::SQLite3)

# Specify any include directories if needed
target_include_directories(db_reader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})