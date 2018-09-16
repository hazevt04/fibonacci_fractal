cmake_minimum_required( VERSION 2.8.9 )

project( fibonacci_fractal )

include_directories( $CMAKE_SOURCE_DIR )

add_executable( fibonacci_fractal fibonacci_fractal.cpp )

target_link_libraries( fibonacci_fractal gsl gslcblas png z m )

