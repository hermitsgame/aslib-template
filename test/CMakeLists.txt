cmake_minimum_required(VERSION 3.5)

project(hello_test)

find_package(GTest REQUIRED)

add_definitions("-DUNIT_TEST")

add_executable(hello_test
              hello_test.cc
	      vec_test.cc)

target_link_libraries(hello_test ${GTEST_BOTH_LIBRARIES})
target_compile_options(hello_test PUBLIC ${GTEST_CFLAGS})

add_test(mytest hello_test)
enable_testing()
