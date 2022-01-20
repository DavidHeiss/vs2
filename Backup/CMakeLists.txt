cmake_minimum_required(VERSION 3.10)

project(Backup)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(conanbuildinfo.cmake)
conan_basic_setup()

set(
    files
    Libary/Server.h
    Libary/TCP/TCPClient.h
    Libary/TCP/TCPRequest.h
    Libary/TCP/TCPResponse.h
    Libary/TCP/TCPServer.h
    Libary/serialize.h
    Libary/message.pb.h
    Libary/message.pb.cc
)

add_executable(Backup main.cpp ${files})
target_link_libraries(Backup ${CONAN_LIBS})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)