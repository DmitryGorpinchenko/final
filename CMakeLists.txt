cmake_minimum_required (VERSION 2.8)

add_compile_options (-std=c++11)

set (SRCS io.cpp server.cpp req.cpp)

add_executable (final main.cpp ${SRCS})
target_link_libraries (final pthread)
