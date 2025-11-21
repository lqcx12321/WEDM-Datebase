cmake_minimum_required(VERSION 3.10)
project(recommend)

add_library(recommend SHARED recommend.cpp recommend_c_api.cpp)
target_include_directories(recommend PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})