cmake_minimum_required(VERSION 3.1)

project(rv_emulator LANGUAGES CXX)

set (CMAKE_CXX_STANDARD 14)

add_executable(rv_emulator src/main.cpp src/decoding.cpp)
