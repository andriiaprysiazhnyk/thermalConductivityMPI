cmake_minimum_required(VERSION 3.14)
project(thermal_conductivity_mpi)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_COMPILER "mpic++")

set(headers headers/config_reader.h headers/finite_differences.h headers/matrix.h headers/utils.h headers/mpi_transactions.h headers/heatmap.h)
set(sources src/config_reader.cpp src/finite_differences.cpp src/matrix.cpp src/mpi_transactions.cpp src/utils.cpp src/heatmap.cpp)

find_package(Boost REQUIRED COMPONENTS mpi)
add_definitions( -DMAGICKCORE_QUANTUM_DEPTH=16 )
add_definitions( -DMAGICKCORE_HDRI_ENABLE=0 )
#add_definitions( profile:skip=ICC)
find_package(ImageMagick REQUIRED COMPONENTS Magick++ convert)
find_package(PNG)
message(${ImageMagick_INCLUDE_DIRS})
set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
include_directories(./headers ${Boost_INCLUDE_DIRS} ${ImageMagick_INCLUDE_DIRS} ${PNG_INCLUDE_DIRS})

add_executable(thermal_conductivity_mpi src/main.cpp ${headers} ${sources})
target_link_libraries(thermal_conductivity_mpi ${Boost_LIBRARIES} ${ImageMagick_LIBRARIES} ${PNG_LIBRARIES})
