cmake_minimum_required (VERSION 3.8)
project(sortserver)

option(test "Build all tests." OFF) # Makes boolean 'test' available.

set(CMAKE_CXX_STANDARD 14)
find_package(CUDA)
if(CUDA_FOUND)
enable_language(CUDA)
endif()
enable_language(CXX)
find_package(Qt5 COMPONENTS Core WebSockets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(SERVER_HEADER_FILES
	sortserver.h
	)

set(SERVER_SOURCE_FILES
	sortserver.cpp
	)

set(CPU_SORT_HEADER_FILES
	cpuMergesort.h
 )

set(CPU_SORT_SRC_FILES
	cpuMergesort.cpp
 )

set(GPU_SORT_HEADER_FILES
	cuMergesort.h
 )

set(GPU_SORT_SRC_FILES
	cuMergesort.cu
 )



    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -O3")
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -rdc=true")
    set(CUDA_NVCC_FLAGS "${CUDA_NVCC_FLAGS} -arch=sm_35")

option(USE_CUDA "Use CUDA" ON)

if(USE_CUDA)
	enable_language("CUDA")
	add_library(sortserver_gpu SHARED mergesort.cu ${GPU_SORT_HEADER_FILES} ${GPU_SORT_SRC_FILES})
	add_executable(sortserver main.cpp ${SERVER_HEADER_FILES} ${SERVER_SOURCE_FILES} ${CPU_SORT_HEADER_FILES} ${CPU_SORT_SRC_FILES})
	target_link_libraries(sortserver sortserver_gpu)
	qt5_use_modules(sortserver Core WebSockets)
        add_executable(checker checker.cpp ${CPU_SORT_HEADER_FILES} ${CPU_SORT_SRC_FILES})
	target_link_libraries(checker sortserver_gpu)
        add_executable(mergesort mergesort.cu)
else(USE_CUDA)
        add_executable(sortserver main.cpp ${SERVER_HEADER_FILES} ${SERVER_SOURCE_FILES} ${CPU_SORT_HEADER_FILES} ${CPU_SORT_SRC_FILES})
        add_executable(checker checker.c ${CPU_SORT_HEADER_FILES} ${CPU_SORT_SRC_FILES})
	qt5_use_modules(sortserver Core WebSockets)
endif()

message(STATUS "USE_CUDA: ${USE_CUDA}")
if(USE_CUDA)
    add_definitions(-DUSE_CUDA)
endif()




install (TARGETS sortserver DESTINATION bin)

if (test)
  find_package(GTest REQUIRED)
  include(GoogleTest)
  enable_testing()

  add_executable(cpu_sort_test cpu_sort_test.cpp cpuMergesort.h cpuMergesort.cpp)
  target_link_libraries(cpu_sort_test ${GTEST_BOTH_LIBRARIES} pthread)
  add_executable(gpu_sort_test gpu_sort_test.cpp)
  target_link_libraries(gpu_sort_test sortserver_gpu ${GTEST_BOTH_LIBRARIES} pthread)


 #  add_test(NAME cpu-sorttest COMMAND cpu_sort_test)
  gtest_add_tests(gpu_sort_test "" AUTO)

endif()

