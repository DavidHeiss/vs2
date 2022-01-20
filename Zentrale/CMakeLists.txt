cmake_minimum_required(VERSION 3.10)

project(Zentrale)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(conanbuildinfo.cmake)
conan_basic_setup()

set(
    files
    Libary/TCP/TCPServer.h
    Libary/TCP/TCPClient.h
    Libary/TCP/TCPRequest.h
    Libary/TCP/TCPResponse.h
    Libary/REST/Path.h
    Libary/REST/Segment.h
    Libary/REST/PathValue.h
    Libary/UDP/UDPServer.h
    Libary/UDP/UDPResponse.h
    Libary/serialize.h
    Libary/message.pb.h
    Libary/message.pb.cc
    src/headquarter/Headquarter.h
    src/headquarter/HeadquarterUDP.h
    src/headquarter/HeadquarterMQTT.h
)

add_executable(Zentrale main.cpp ${files})
target_link_libraries(Zentrale ${CONAN_LIBS})

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)