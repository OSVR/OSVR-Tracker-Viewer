cmake_minimum_required(VERSION 2.8.12)
project(OSVRTrackerView)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

include(EnableExtraCompilerWarnings)
globally_enable_extra_compiler_warnings()

find_package(osvr REQUIRED)
find_package(Boost REQUIRED)
find_package(OpenSceneGraph COMPONENTS OpenThreads osgDB osgViewer osgGA osgUtil REQUIRED)

include_directories(SYSTEM ${Boost_INCLUDE_DIR} ${OPENSCENEGRAPH_INCLUDE_DIRS})

if(WIN32)
    set(TRACKER_VIEW_RESOURCE osvr_tracker_view.rc)
else()
    set(TRACKER_VIEW_RESOURCE)
endif()

add_executable(OSVRTrackerView
    ${TRACKER_VIEW_RESOURCE}
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
set(CMAKE_INSTALL_DOCDIR .)

install(TARGETS OSVRTrackerView
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime)
install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/RPAxes.osg"
        DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT Runtime)


# For generating documentation in HTML
if(WIN32)
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third-party/discount-windows-bins")
endif()
find_package(Markdown)

###
# Docs
###
# Convert the README to HTML if we can
set(MARKDOWN_INPUT README.md CONTRIBUTING.md)
if(MARKDOWN_FOUND)
    include(UseMarkdown)
    add_markdown_target(markdown_docs "${CMAKE_CURRENT_BINARY_DIR}" ${MARKDOWN_INPUT})

    install_markdown_target(markdown_docs DESTINATION ${CMAKE_INSTALL_DOCDIR})
else()
    install(FILES ${MARKDOWN_INPUT} DESTINATION ${CMAKE_INSTALL_DOCDIR})
endif()

install(FILES LICENSE NOTICE DESTINATION ${CMAKE_INSTALL_DOCDIR})