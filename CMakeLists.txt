cmake_minimum_required(VERSION 3.16)

project(Server)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAG "${CMAKE_CXX_FLAG} -pthread")
set(SRC_DIR "${CMKAE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(threadpool src/threadpool.cpp)
target_include_directories(threadpool PUBLIC include)

add_executable(Server main.cpp)
target_link_libraries(Server PRIVATE threadpool Threads::Threads)