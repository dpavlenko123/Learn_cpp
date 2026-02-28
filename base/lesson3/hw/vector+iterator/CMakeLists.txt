cmake_minimum_required(VERSION 3.30)
project(vector_test)

find_package(GTest REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -fuse-ld=lld -lc++ -lc++abi")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall -Wextra -fsanitize=address -fsanitize=undefined -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -fuse-ld=lld -lc++ -lc++abi -fsanitize=address -fsanitize=undefined")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(vector_test tests.cpp)

target_include_directories(vector_test PRIVATE
	${CMAKE_SOURCE_DIR}
)

target_link_libraries(vector_test GTest::gtest GTest::gtest_main)