cmake_minimum_required(VERSION 3.30)
project(string_builder)

find_package(GTest REQUIRED)

add_executable(string_builder tests/string_builder.cpp)

target_include_directories(string_builder PRIVATE
	${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/tests
)

target_link_libraries(string_builder GTest::gtest GTest::gtest_main)