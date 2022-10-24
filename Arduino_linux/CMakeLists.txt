cmake_minimum_required(VERSION 2.8)
project(Communication)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(Communication main.cpp)
target_link_libraries(Communication ${OpenCV_LIBS})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(wiringPi DEFAULT_MSG WIRINGPI_LIBRARIES WIRINGPI_INCLUDE_DIRS)

find_package(WiringPi REQUIRED)
find_package(Threads REQUIRED) 

include_directories(${WIRINGPI_INCLUDE_DIRS}) 

target_link_libraries(Communication ${WIRINGPI_LIBRARIES}) 
target_link_libraries(Communication ${CMAKE_THREAD_LIBS_INIT}) 