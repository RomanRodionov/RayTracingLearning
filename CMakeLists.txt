cmake_minimum_required(VERSION 3.0.0)
project(RayTracing VERSION 0.1.0 LANGUAGES C CXX)

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -fopenmp") 

include(CTest)
enable_testing()

add_executable(RayTracing main.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
