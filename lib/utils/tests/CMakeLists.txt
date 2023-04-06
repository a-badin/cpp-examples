add_executable(utils_test src/test_split_string.cpp)
target_link_libraries(utils_test utils
                                 GTest::gtest_main)

add_test(utils_test utils_test)
