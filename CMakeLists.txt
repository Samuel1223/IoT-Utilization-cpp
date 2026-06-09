cmake_minimum_required(VERSION 3.16)
project(iot_utilization_cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(iot_utilization STATIC
  src/intervals.cpp
  src/utilization.cpp
)
target_include_directories(iot_utilization PUBLIC include)
