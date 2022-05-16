cmake_minimum_required( VERSION 2.8.9 )

project( fibonacci_fractal )

set(EXEC_NAME "${CMAKE_PROJECT_NAME}") 


add_definitions(-std=c++11 )

set(SRCS
   "src/FibFractalInput.cpp"
   "src/FibFractalInputParser.cpp"
   "src/FibFractalBuilder.cpp"
   "src/PNGRenderer.cpp"
   "src/ImageData.cpp"
   "src/main.cpp"
)

add_executable( "${EXEC_NAME}" "${SRCS}" )

target_include_directories( "${EXEC_NAME}" PUBLIC 
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>  # <prefix>/include   
)

target_link_libraries( "${EXEC_NAME}" png z )

install( TARGETS "${EXEC_NAME}" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin" )
