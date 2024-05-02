cmake_minimum_required (VERSION 3.0.2)
project (FaceDetection)
 
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )
include_directories( . )
 
add_executable (${PROJECT_NAME} FaceDetection.cpp )
target_link_libraries (${PROJECT_NAME} ${OpenCV_LIBS})

