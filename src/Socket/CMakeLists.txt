cmake_minimum_required(VERSION 3.15)
project(GNetworking-Socket)

file(GLOB SOURCES "*.cpp")
add_library(GNetworking ${SOURCES})
target_include_directories(GNetworking PRIVATE ${CMAKE_SOURCE_DIR}/include)
