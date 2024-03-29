cmake_minimum_required(VERSION 3.5)
project(GNetworking)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(BUILD_TESTS FALSE)

file(GLOB SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE include)

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE wsock32 ws2_32)
endif(WIN32)

if(BUILD_TESTS)
  add_executable(${PROJECT_NAME}_Server "tests/Server.cpp")
  add_executable(${PROJECT_NAME}_Client "tests/Client.cpp")

  target_include_directories(${PROJECT_NAME}_Server PRIVATE include)
  target_link_libraries(${PROJECT_NAME}_Server PRIVATE ${PROJECT_NAME})

  target_include_directories(${PROJECT_NAME}_Client PRIVATE include)
  target_link_libraries(${PROJECT_NAME}_Client PRIVATE ${PROJECT_NAME})
endif()
