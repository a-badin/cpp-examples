cmake_minimum_required(VERSION 3.20)
project(network)

add_compile_options(-Wall -Wextra)
add_executable(client src/client.cpp)
target_include_directories(client PRIVATE include)

add_executable(server src/server.cpp)
target_include_directories(server PRIVATE include)
