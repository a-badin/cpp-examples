cmake_minimum_required(VERSION 3.20)
project(threading)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Boost REQUIRED COMPONENTS system)

set(CMAKE_AUTOMOC YES)

#add_compile_options(-fsanitize=thread)
#add_link_options(-fsanitize=thread)
add_executable(main src/main.cpp
                    src/main_window.cpp
                    include/main_window.hpp)

target_include_directories(main PRIVATE include
                                        beast-boost-1.74.0/include)

target_link_libraries(main PRIVATE Qt6::Widgets Boost::system)
