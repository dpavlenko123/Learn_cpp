cmake_minimum_required(VERSION 3.30)
project(compiler)

find_package(GTest REQUIRED)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -fuse-ld=lld -lc++ -lc++abi")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall -Wextra -fsanitize=address -fsanitize=undefined -stdlib=libc++")
set(CMAKE_CXX_FLAGS_DEBUG": "-g -O0 -Wall -Wextra -fsanitize=thread)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++ -fuse-ld=lld -lc++ -lc++abi -fsanitize=address -fsanitize=undefined")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(compiler src/ast.cpp src/main.cpp)

target_include_directories(compiler PRIVATE
	${CMAKE_SOURCE_DIR}
)

target_link_libraries(compiler GTest::gtest GTest::gtest_main)