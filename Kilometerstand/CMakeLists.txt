cmake_minimum_required(VERSION 3.10)
enable_testing()

project(Kilometerstand)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(conanbuildinfo.cmake)
conan_basic_setup()

set(
    files
    src/Kilometerstand.h
    src/KilometerstandUDP.h
    Libary/TCP/TCPResponse.h
    Libary/message.pb.h
    Libary/message.pb.cc
)

add_executable(Kilometerstand main.cpp ${files})
target_link_libraries(Kilometerstand ${CONAN_LIBS})

add_executable(TestKilometerstand src/tests/Kilometerstand.cpp ${files})
target_link_libraries(TestKilometerstand ${CONAN_LIBS})
add_test(NAME Kilometerstand COMMAND ./bin/TestKilometerstand)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)