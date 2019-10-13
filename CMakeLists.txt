cmake_minimum_required(VERSION 3.2)
project(
    regula
    VERSION 1.0.0
    LANGUAGES CXX
)

add_library(regula INTERFACE)

target_compile_features(regula INTERFACE cxx_std_17)

target_include_directories(regula INTERFACE include/)