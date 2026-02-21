cmake_minimum_required(VERSION 3.16)
project(my_vector LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(my_vector tests.cpp)

target_include_directories(my_vector PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(my_vector PRIVATE
    gtest
    gtest_main
)