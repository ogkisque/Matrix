set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)

add_library(matrix_lib INTERFACE)
target_include_directories(matrix_lib INTERFACE ${INCLUDE_DIR})

add_executable(main main.cpp)
target_link_libraries(main matrix_lib)