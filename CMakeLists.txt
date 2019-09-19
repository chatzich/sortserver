cmake_minimum_required (VERSION 3.3)
project(sortserver)

set(CMAKE_CXX_STANDARD 14)
find_package(CUDA)
enable_language(CUDA)
enable_language(CXX)
find_package(Qt5 COMPONENTS Core WebSockets REQUIRED)


message(${CUDA_INCLUDE_DIRS})

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


find_package(Qt5 COMPONENTS Core Widgets Sql Concurrent REQUIRED)

set(SORT_HEADER_FILES
	sortserver.h
 )

set(SORT_SRC_FILES
	sortserver.cpp
 )


enable_testing()
add_executable(sortserver main.cpp ${SORT_HEADER_FILES} ${SORT_SRC_FILES})

#add_executable(fibonacci_generator_test WIN32 ${DELPHIS_HEADER_FILES} ${TESTS_SRC_DIR}/fibonacci_generator_test.cpp )
#add_test(NAME fibonacci_generator_test COMMAND fibonacci_generator_test)

qt5_use_modules(sortserver Core WebSockets)
