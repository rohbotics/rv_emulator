cmake_minimum_required(VERSION 3.5)
project(rv_emulator LANGUAGES CXX)
set (CMAKE_CXX_STANDARD 14)

include_directories(src)

add_subdirectory(external/fmt)

add_library(rv STATIC src/decoding.cpp src/registers.cpp)
target_link_libraries(rv PUBLIC fmt::fmt)
target_compile_options(rv PRIVATE -Wall -Wextra -pedantic)

add_executable(rv_emulator src/main.cpp)
target_link_libraries(rv_emulator PUBLIC rv fmt::fmt)
target_compile_options(rv_emulator PRIVATE -Wall -Wextra -pedantic)


## TESTING ##
add_subdirectory(external/Catch2)
enable_testing()

# Add 'make check' which shows he output of catch on failure
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)

add_library(test_main tests/test_main.cpp)
target_link_libraries(test_main PUBLIC Catch2::Catch2)

add_executable(rv_decoder_tests tests/decoder_test.cpp)
target_link_libraries(rv_decoder_tests test_main rv)
add_test(NAME DecoderTests COMMAND rv_decoder_tests)

add_executable(rv_format_tests tests/format_test.cpp)
target_link_libraries(rv_format_tests test_main rv)
add_test(NAME FormatTests COMMAND rv_format_tests)

add_executable(rv_registers_test tests/registers_test.cpp)
target_link_libraries(rv_registers_test test_main rv)
add_test(NAME RegisterTests COMMAND rv_registers_test)

add_executable(rv_tests tests/format_test.cpp tests/decoder_test.cpp tests/registers_test.cpp)
target_link_libraries(rv_tests test_main rv)

if(ENABLE_COVERAGE)
if(CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)
    include(CodeCoverage)
    APPEND_COVERAGE_COMPILER_FLAGS()
    set(COVERAGE_LCOV_EXCLUDES "/usr/include/\\*" '${CMAKE_SOURCE_DIR}/external/*' "*tests/*")
    setup_target_for_coverage_lcov(NAME ${PROJECT_NAME}_coverage EXECUTABLE rv_tests DEPENDENCIES rv_tests)
endif()
endif()
