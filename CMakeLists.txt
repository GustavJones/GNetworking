cmake_minimum_required(VERSION 3.5)
project(Sockets)

file(GLOB SOURCES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SOURCES})
add_executable(${PROJECT_NAME}_test "src/main.cpp")
add_executable(${PROJECT_NAME}_test_c "src/main_c.cpp")

target_include_directories(${PROJECT_NAME} PRIVATE include)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE wsock32 ws2_32)
endif(WIN32)

target_include_directories(${PROJECT_NAME}_test PRIVATE include)
target_link_libraries(${PROJECT_NAME}_test PRIVATE ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}_test_c PRIVATE include)
target_link_libraries(${PROJECT_NAME}_test_c PRIVATE ${PROJECT_NAME})