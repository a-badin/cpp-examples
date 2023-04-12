add_library(imdb_localizator src/imdb_localizator.cpp)
target_include_directories(imdb_localizator PUBLIC include)
target_link_libraries(imdb_localizator PUBLIC utils
                                              PkgConfig::LIBCONFIG)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
