cmake_minimum_required(VERSION 2.8.12)
project(VRPNTrackerView)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/CMakeBoostHelper.cmake")
find_package(Boost REQUIRED)
find_package(VRPN REQUIRED)
find_package(OpenSceneGraph COMPONENTS OpenThreads osgDB osgViewer osgGA osgUtil REQUIRED)

include_directories(${Boost_INCLUDE_DIR} ${VRPN_INCLUDE_DIRS} ${OPENSCENEGRAPH_INCLUDE_DIRS})

add_executable(VRPNTrackerView
    OSGQuatlib.h
    VRPNMainloop.h
    VRPNNodeData.h
    VRPNUpdateCallback.cpp
    VRPNUpdateCallback.h
    VRPNTrackerView.cpp)
target_link_libraries(VRPNTrackerView ${VRPN_LIBRARIES} ${OPENSCENEGRAPH_LIBRARIES})
