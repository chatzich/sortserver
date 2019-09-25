cmake_minimum_required (VERSION 3.3)
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



set(SORT_HEADER_FILES
	sortserver.h
	cpuMergesort.h
 )

set(SORT_SRC_FILES
	sortserver.cpp
	cpuMergesort.cpp
 )

if(CUDA_FOUND)
set(SORT_HEADER_FILES ${SORT_HEADER_FILES} cuMergesort.h)
set(SORT_SRC_FILES ${SORT_SRC_FILES} cuMergesort.cu)
endif()


add_executable(sortserver main.cpp ${SORT_HEADER_FILES} ${SORT_SRC_FILES})

if(CUDA_FOUND)
target_link_libraries(sortserver ${CUDA_LIBRARIES})
endif()

qt5_use_modules(sortserver Core WebSockets)

install (TARGETS sortserver DESTINATION bin)

if (test)
	find_package(GTest REQUIRED)
  include(GoogleTest)
  enable_testing()

  add_executable(cpu_sort_test test.cpp cpuMergesort.h cpuMergesort.cpp)
  target_link_libraries(cpu_sort_test ${GTEST_BOTH_LIBRARIES} pthread)


  #  add_test(NAME cpu-sorttest COMMAND cpu_sort_test)
  gtest_add_tests(cpu_sort_test "" AUTO)

endif()

