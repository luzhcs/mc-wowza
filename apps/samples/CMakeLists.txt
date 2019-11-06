project(image_predictor)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

if(OpenCV_FOUND)
   message("Found OpenCV")
   message("Includes: " ${OpenCV_INCLUDE_DIRS})
endif(OpenCV_FOUND)

find_package( OpenCV REQUIRED )

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/include)
LINK_DIRECTORIES(${CMAKE_SOURCE_DIR}/lib)
LINK_LIBRARIES ( ${OpenCV_LIBS} pthread)

set(CMAKE_BUILD_TYPE Debug)
SET ( SRC_FILES main.cpp src/image_predictor.cpp)
set(LIBRARY_OUTPUT_PATH lib)
ADD_LIBRARY(imagePredictor SHARED ${SRC_FILES})
add_executable(iPredictor ${SRC_FILES})

#set(EXECUTABLE_OUTPUT_PATH bin)
