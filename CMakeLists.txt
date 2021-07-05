cmake_minimum_required(VERSION 3.16)
project(NetworkProgrammingWithC)

set(CMAKE_CXX_STANDARD 17)

add_executable(server server.cpp)
add_executable(client client.cpp)
add_executable(http_server http_server.cpp)

target_link_libraries(server PRIVATE ws2_32)
target_link_libraries(client PRIVATE ws2_32)
target_link_libraries(http_server PRIVATE ws2_32)