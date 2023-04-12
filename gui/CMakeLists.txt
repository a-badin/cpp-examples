cmake_minimum_required(VERSION 3.20)

project(example)


find_package(PkgConfig)

pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-3.0)
pkg_check_modules(LIBCONFIG REQUIRED IMPORTED_TARGET libconfig++)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

add_subdirectory(lib/utils)
add_subdirectory(lib/imdb_localizator)

add_executable(main src/main.cpp
                    src/gtkwindow.cpp
                    src/qtwindow.cpp
                    src/qtwindow.ui)


target_link_libraries(main PkgConfig::GTKMM
                           PkgConfig::LIBCONFIG
                           Qt6::Widgets
                           imdb_localizator)

