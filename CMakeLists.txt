cmake_minimum_required(VERSION 3.20.0 FATAL_ERROR)

project(SchwungSSR VERSION 0.1.0 DESCRIPTION "A server-side rendering library for C++" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(SCHWUNG_SSR_BUILD_TEST "Build the test application?" ON)

add_library(SchwungSSR "SchwungSSR.cpp" "SchwungSSR.hpp")

if(SCHWUNG_SSR_BUILD_TEST)
    add_executable(SchwungSSR_test "test.cpp")
    target_link_libraries(SchwungSSR_test SchwungSSR)
endif()
