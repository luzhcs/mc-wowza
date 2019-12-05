project(ipcam-capture)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

find_package( OpenCV REQUIRED )
find_package( CURL REQUIRED)

if(OpenCV_FOUND)
   message("Found OpenCV")
   message("Includes: " ${OpenCV_INCLUDE_DIRS})
endif(OpenCV_FOUND)

if(CURL_FOUND)
   message("Found CURL")
   message("Includes: " ${CURL_INCLUDE_DIRS})
endif(CURL_FOUND)

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include ${OpenCV_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)
LINK_LIBRARIES ( ${OpenCV_LIBS} ${CURL_LIBRARIES} pthread)
SET ( SRC_FILES src/main.cpp src/ipcam.hpp src/ipcam.cpp)
SET ( CMAKE_VERBOSE_MAKEFILE true )
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")

add_executable(cap ${SRC_FILES})