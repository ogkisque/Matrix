add_executable(tests tests.cpp)

target_link_libraries(tests GTest::gtest_main)
target_link_libraries(tests matrix_lib)
target_include_directories(tests PRIVATE ${GMOCK_INCLUDE_DIRS} ${GTEST_INCLUDE_DIRS})

set(RUN_TESTS ./tests --gtest_color=yes)
add_test(
    NAME unit_test
    COMMAND ${RUN_TESTS}
)