file(GLOB SOCKET_SOURCES "src/Socket/*.cpp")
file(GLOB TCP_SOURCES "src/TCP/*.cpp")

add_library(GNetworking ${SOCKET_SOURCES} ${TCP_SOURCES})
target_include_directories(GNetworking PUBLIC "${CMAKE_SOURCE_DIR}/include")

if(DEFINED WIN32)
  target_link_libraries(GNetworking PRIVATE Ws2_32.lib)
endif()
