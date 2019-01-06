cmake_minimum_required( VERSION 2.8.9 )

project( fibonacci_fractal )

include_directories( $CMAKE_SOURCE_DIR )

add_definitions(-std=c++11 )

set(SRCS
   "FibFractalInput.cpp"
   "FibFractalInputParser.cpp"
   "FibFractalBuilder.cpp"
   "ImageData.cpp"
   "main.cpp"
)


add_executable( fibonacci_fractal "${SRCS}" )

target_link_libraries( fibonacci_fractal png z )

