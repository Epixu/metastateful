cmake_minimum_required(VERSION 3.5)

project(test VERSION 1.0 LANGUAGES CXX)

add_executable(test main.cpp order1.cpp order2.cpp)
target_compile_features(test PUBLIC cxx_std_23)
