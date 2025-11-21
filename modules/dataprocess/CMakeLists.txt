project(dataprocess)

add_library(dataprocess SHARED dataprocess.cpp dataprocess_c_api.cpp)
target_include_directories(dataprocess PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})