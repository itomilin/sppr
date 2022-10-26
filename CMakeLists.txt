cmake_minimum_required(VERSION 3.5)

project(sppr LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(USE_IAC  "IAC SPPR" OFF)
option(USE_ORCH "ORCH SPPR" OFF)
option(USE_TEST "TEST SPPR" OFF)

if (USE_IAC)
    add_definitions(-DIAC)
endif()

if (USE_ORCH)
    add_definitions(-DORCH)
endif()

if (USE_TEST)
    add_definitions(-DTEST)
endif()

add_executable(sppr main.cpp)
