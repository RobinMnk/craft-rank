add_executable(craft_rank_app main.cpp)

# Link libraries
target_link_libraries(craft_rank_app db_reader rank_algo)

target_compile_options(craft_rank_app PUBLIC -Wall -Wextra)