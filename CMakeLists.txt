cmake_minimum_required(VERSION 2.8.12)
project(OSVRTrackerView)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR})

include(CMakeBoostHelper)
include(EnableExtraCompilerWarnings)
globally_enable_extra_compiler_warnings()

find_package(Boost REQUIRED)
find_package(osvr REQUIRED)
find_package(OpenSceneGraph COMPONENTS OpenThreads osgDB osgViewer osgGA osgUtil REQUIRED)

include_directories(SYSTEM ${Boost_INCLUDE_DIR} ${OPENSCENEGRAPH_INCLUDE_DIRS})

add_executable(OSVRTrackerView
    OSGMathInterop.h
    OSGMathInterop.h
    OSVRContext.cpp
    OSVRContext.h
    OSVRInterfaceData.cpp
    OSVRInterfaceData.h
    OSVRUpdateCallback.cpp
    OSVRUpdateCallback.h
    OSVRTrackerView.cpp)
target_link_libraries(OSVRTrackerView osvr::osvrClientKitCpp ${OPENSCENEGRAPH_LIBRARIES})

set(CMAKE_INSTALL_BINDIR .)

install(TARGETS OSVRTrackerView
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/RPAxes.osg"
        DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime)