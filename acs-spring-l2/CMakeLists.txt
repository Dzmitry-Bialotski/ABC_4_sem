cmake_minimum_required(VERSION 3.14)
project(riscv_sim)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(lib/googletest-master)
include_directories(lib/googletest-master/googletest/include)
include_directories(lib/googletest-master/googlemock/include)

file(GLOB SRC
        "src/*.h"
        "src/*.cpp"
        )

add_executable(riscv_sim ${SRC} Tests/test.cpp)
target_link_libraries(riscv_sim gtest gtest_main)