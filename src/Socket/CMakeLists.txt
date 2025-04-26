cmake_minimum_required(VERSION 3.15)
project(GNetworking-Socket)

file(GLOB SOURCES "*.cpp")
add_library(GNetworking ${SOURCES})
target_include_directories(GNetworking PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../../include)

if(DEFINED WIN32)
  target_link_libraries(GNetworking PRIVATE Ws2_32.lib)
endif()
