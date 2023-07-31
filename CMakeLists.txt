cmake_minimum_required(VERSION 3.22)
project(fail2drop)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

find_package(Boost REQUIRED)

set(BUILD_EXAMPLE OFF)
set(BUILD_TEST OFF)
set(BUILD_STATIC_LIBS ON)
set(BUILD_SHARED_LIBS OFF)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/inotify-cpp)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/spdlog)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inotify-cpp/src/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/spdlog/include)


add_executable(fail2drop main.cpp src/fail2drop.cpp src/include/fail2drop.h src/util.cpp src/include/util.h)
target_link_libraries(fail2drop PUBLIC inotify-cpp-static spdlog)