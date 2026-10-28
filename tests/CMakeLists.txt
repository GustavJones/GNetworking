cmake_minimum_required(VERSION 3.15)
project(GNetworking-Tests)

add_executable(Server "Server.cpp")
target_link_libraries(Server PRIVATE GNetworking)

add_executable(Client "Client.cpp")
target_link_libraries(Client PRIVATE GNetworking)
