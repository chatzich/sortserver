cmake_minimum_required (VERSION 3.3)
project(sortserver)

set(CMAKE_CXX_STANDARD 14)
find_package(CUDA)
if(CUDA_FOUND)
enable_language(CUDA)
endif()
enable_language(CXX)
find_package(Qt5 COMPONENTS Core WebSockets REQUIRED)

message(${CUDA_LIBRARIES})
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
	cpuMergesort.cpp
 )
if(CUDA_FOUND)
set(SORT_HEADER_FILES ${SORT_HEADER_FILES} cuMergesort.h)
set(SORT_SRC_FILES ${SORT_SRC_FILES} cuMergesort.cu)
endif()


enable_testing()
add_executable(sortserver main.cpp ${SORT_HEADER_FILES} ${SORT_SRC_FILES})

if(CUDA_FOUND)
target_link_libraries(sortserver ${CUDA_LIBRARIES})
endif()

qt5_use_modules(sortserver Core WebSockets)
