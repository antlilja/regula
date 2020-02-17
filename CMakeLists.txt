cmake_minimum_required(VERSION 3.2)
project(
    regula
    VERSION 1.0.0
    LANGUAGES CXX
)

option(REGULA_BUILD_TESTS "Build tests" OFF)

add_library(regula INTERFACE)
add_library(regula::regula ALIAS regula)

target_compile_features(regula INTERFACE cxx_std_17)

target_include_directories(regula INTERFACE include/)

if(REGULA_BUILD_TESTS)
    add_subdirectory(tests)
endif()