cmake_minimum_required(VERSION 2.8.12)
project(OSVRTrackerView)

include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeBoostHelper.cmake")
find_package(Boost REQUIRED)
find_package(osvr REQUIRED)
find_package(OpenSceneGraph COMPONENTS OpenThreads osgDB osgViewer osgGA osgUtil REQUIRED)

include_directories(${Boost_INCLUDE_DIR} ${OPENSCENEGRAPH_INCLUDE_DIRS})

add_executable(OSVRTrackerView OSVRTrackerView.cpp)
target_link_libraries(OSVRTrackerView osvr::osvrClientKitCpp ${OPENSCENEGRAPH_LIBRARIES})
