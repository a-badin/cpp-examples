add_executable(imdb_localizator_test src/test_imdb_localizator.cpp)
target_link_libraries(imdb_localizator_test imdb_localizator
                                            GTest::gtest_main
                                            GTest::gmock)
                                            

add_test(NAME imdb_localizator_test COMMAND imdb_localizator_test
                                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
