cmake_minimum_required(VERSION 3.10)
enable_testing()

project(Verkehrssituation)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(conanbuildinfo.cmake)
conan_basic_setup()

set(
    files
    src/Verkehrssituation.h
    src/VerkehrssituationUDP.h
    src/VerkehrssituationMQTT.h
    Libary/message.pb.h
    Libary/message.pb.cc
)

add_executable(Verkehrssituation main.cpp ${files})
target_link_libraries(Verkehrssituation ${CONAN_LIBS})

add_executable(TestVerkehrssituation src/tests/Verkehrssituation.cpp ${files})
target_link_libraries(TestVerkehrssituation ${CONAN_LIBS})
add_test(NAME TestVerkehrssituation COMMAND ./bin/TestVerkehrssituation)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)