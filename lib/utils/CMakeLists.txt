add_library(utils src/utils.cpp)
target_include_directories(utils PUBLIC include)

if(BUILD_TESTS)
  add_subdirectory(tests)
endif()
