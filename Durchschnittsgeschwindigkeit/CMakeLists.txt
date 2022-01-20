cmake_minimum_required(VERSION 3.10)
enable_testing()

project(Durchschnittsgeschwindigkeit)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(conanbuildinfo.cmake)
conan_basic_setup()

set(
    files
    src/Durchschnittsgeschwindigkeit.h
    src/DurchschnittsgeschwindigkeitUDP.h
    Libary/TCP/TCPResponse.h
    Libary/message.pb.h
    Libary/message.pb.cc
)

add_executable(Durchschnittsgeschwindigkeit main.cpp ${files})
target_link_libraries(Durchschnittsgeschwindigkeit ${CONAN_LIBS})

add_executable(TestDurchschnittsgeschwindigkeit src/tests/Durchschnittsgeschwindigkeit.cpp ${files})
target_link_libraries(TestDurchschnittsgeschwindigkeit ${CONAN_LIBS})
add_test(NAME TestDurchschnittsgeschwindigkeit COMMAND ./bin/TestDurchschnittsgeschwindigkeit)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)