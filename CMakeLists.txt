cmake_minimum_required(VERSION 3.15)

project(ConcurrentQueues
        DESCRIPTION "A small implementation of thread safe data containers"
        LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-Wall")

add_executable(cbq_test cbq_test.cpp)
target_link_libraries(cbq_test pthread)
add_executable(cs_test cs_test.cpp)
target_link_libraries(cs_test pthread)
add_executable(cq_test cq_test.cpp)
target_link_libraries(cq_test pthread)

enable_testing()
add_test(NAME cbq_test COMMAND cbq_test)
add_test(NAME cs_test COMMAND cs_test)
add_test(NAME cq_test COMMAND cq_test)
set_tests_properties(cbq_test cs_test cq_test PROPERTIES TIMEOUT 1)
