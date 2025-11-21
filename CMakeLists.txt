cmake_minimum_required(VERSION 3.10)
project(wedm_recommender)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Qt5 COMPONENTS Widgets Sql REQUIRED)

add_subdirectory(app)
add_subdirectory(modules/dataprocess)
add_subdirectory(modules/recommend)