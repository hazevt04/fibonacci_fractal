cmake_minimum_required( VERSION 2.8.9 )

project( fibonacci_fractal LANGUAGES C )

set(EXEC_NAME "${CMAKE_PROJECT_NAME}")

add_executable( "${EXEC_NAME}" fibonacci_fractal.c )

target_include_directories( "${EXEC_NAME}" PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>  # <prefix>/include
)

target_link_libraries( "${EXEC_NAME}" png z m )

install( TARGETS "${EXEC_NAME}" DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin" )
