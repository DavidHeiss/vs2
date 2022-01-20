cmake_minimum_required(VERSION 3.10)
enable_testing()

project(Fuellstand)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(conanbuildinfo.cmake)
conan_basic_setup()

set(
    files
    src/Fuellstand.h
    src/FuellstandUDP.h
    src/FuellstandMQTT.h
    Libary/TCP/TCPResponse.h
    Libary/message.pb.h
    Libary/message.pb.cc
    Libary/serialize.h
)

add_executable(Fuellstand main.cpp ${files})
target_link_libraries(Fuellstand ${CONAN_LIBS})

add_executable(TestFuellstand src/tests/Fuellstand.cpp ${files})
target_link_libraries(TestFuellstand ${CONAN_LIBS})
add_test(NAME TestFuellstand COMMAND ./bin/TestFuellstand)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)