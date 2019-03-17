cmake_minimum_required(VERSION 3.13)
project(roiclient)

set(CMAKE_CXX_STANDARD 14)

include_directories(lib/rsocket)

add_executable(roiclient
        lib/rsocket/client_socket.cpp
        lib/rsocket/client_socket.h
        roiclient.cpp)
