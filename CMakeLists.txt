cmake_minimum_required(VERSION 3.0)
project(mymuduo)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++20 -fPIC")
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/include/spdlog)
add_subdirectory(test)

# set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
# aux_source_directory(. SRC_LIST)
# add_library(mymuduo SHARED ${SRC_LIST})