cmake_minimum_required(VERSION 2.8)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") # -DVERBOSE")

ADD_DEFINITIONS("-DMIXED")

SET(sources
dgramoverstream.cpp
framing.cpp
inetbase.cpp
inetclientstream.cpp
inetserverdgram.cpp
select.cpp
streamclient.cpp
unixclientdgram.cpp
unixdgram.cpp
unixserverstream.cpp
exception.cpp
inetclientdgram.cpp
inetdgram.cpp
inetserverstream.cpp
socket.cpp
unixbase.cpp
unixclientstream.cpp
unixserverdgram.cpp
)

ADD_DEFINITIONS(-fPIC) # for the static library which needs to be linked into the shared libsocket++.so object.


TARGET_LINK_LIBRARIES(socket++ socket_int)

INSTALL(TARGETS socket++ DESTINATION ${LIB_DIR})
ENDIF()


SET_TARGET_PROPERTIES(socket++_int PROPERTIES OUTPUT_NAME socket++)

TARGET_LINK_LIBRARIES(socket++_int socket_int)

INSTALL(TARGETS socket++_int DESTINATION ${LIB_DIR})
ENDIF()

project(Server)
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
add_executable(Server main.cpp)
target_link_libraries(Server ${OpenCV_LIBS})
set(CMAKE_EXTRA_INCLUDE_FILES sys/socket.h)
