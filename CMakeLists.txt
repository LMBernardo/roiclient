cmake_minimum_required(VERSION 3.13)
project(roiclient)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib/client_socket_handler)

add_executable(roiclient
        lib/client_socket_handler/client_socket_handler.cpp
        lib/client_socket_handler/client_socket_handler.h
        roiclient.cpp)
