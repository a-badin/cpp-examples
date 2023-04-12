cmake_minimum_required(VERSION 3.10)
project(imdb)

find_package(PkgConfig)
pkg_check_modules(LIBCONFIG REQUIRED IMPORTED_TARGET libconfig++)


option(BUILD_TESTS "build tests")
if(BUILD_TESTS)
  enable_testing()
  find_package(GTest REQUIRED)
endif()

add_subdirectory(lib/utils)
add_subdirectory(lib/imdb_localizator)

add_executable(imdb src/main.cpp)
target_link_libraries(imdb imdb_localizator)

