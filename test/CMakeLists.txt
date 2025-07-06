add_executable(parser_test parser_test.cpp)

target_link_libraries(parser_test PRIVATE
    kx_converter_lib
    GTest::gtest_main
)

include(GoogleTest)
gtest_add_tests(TARGET parser_test)