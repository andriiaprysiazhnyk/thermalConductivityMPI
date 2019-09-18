cmake_minimum_required(VERSION 3.14)
project(thermal_conductivity_mpi)

set(CMAKE_CXX_STANDARD 14)

include_directories(./headers)

add_executable(thermal_conductivity_mpi src/main.cpp src/matrix.cpp headers/matrix.h headers/finite_differences.h src/finite_differences.cpp headers/config_reader.h src/config_reader.cpp)
