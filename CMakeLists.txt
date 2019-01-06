cmake_minimum_required(VERSION 3.5)
project(rv_emulator LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 14)

include_directories(src)

add_subdirectory(external/fmt)

add_executable(rv_emulator src/main.cpp src/decoding.cpp)
target_link_libraries(rv_emulator fmt::fmt)
target_compile_options(rv_emulator PRIVATE -Wall -Wextra -pedantic)


## TESTING ##
add_subdirectory(external/Catch2)
enable_testing()

# Add 'make check' which shows he output of catch on failure
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)

add_executable(rv_decoder_tests tests/test_main.cpp tests/decoder_test.cpp src/decoding.cpp)
target_link_libraries(rv_decoder_tests Catch2::Catch2)
target_link_libraries(rv_decoder_tests fmt::fmt)
add_test(NAME DecoderTests COMMAND rv_decoder_tests)
