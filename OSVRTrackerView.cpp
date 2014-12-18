/** @file
    @brief Implementation

    @date 2014

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2014 Sensics, Inc.
//
// All rights reserved.
//
// (Final version intended to be licensed under
// the Apache License, Version 2.0)

// Internal Includes
// - none

// Library/third-party includes
#include <osvr/ClientKit/ClientKit.h>

#include <osg/ref_ptr>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeCallback>

#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>

// Standard includes
#include <iostream>

class OSVRUpdateCallback : public osg::NodeCallback {

};

int main(int argc, char **argv) {
    /// Parse arguments
    osg::ArgumentParser args(&argc, argv);
    args.getApplicationUsage()->setApplicationName(args.getApplicationName());
    args.getApplicationUsage()->setDescription(
        args.getApplicationName() +
        " is a tool for visualizing tracking data from the OSVR system.");
    // args.getApplicationUsage()->setCommandLineUsage(args.getApplicationName()
    // + " [options] osvrpath ...");

    /// Init the OSG viewer
    osgViewer::Viewer viewer(args);

    osg::ApplicationUsage::Type helpType;
    if ((helpType = args.readHelpType()) != osg::ApplicationUsage::NO_HELP) {
        args.getApplicationUsage()->write(std::cerr);
        return 1;
    }

    if (args.errors()) {
        args.writeErrorMessages(std::cerr);
        return 1;
    }

    args.reportRemainingOptionsAsUnrecognized();

    if (args.errors()) {
        args.writeErrorMessages(std::cerr);
        return 1;
    }

    /// Set up OSVR
    osvr::clientkit::ClientContext ctx("org.opengoggles.osvrtrackerview");

    osg::ref_ptr<osg::PositionAttitudeTransform> scene =
        new osg::PositionAttitudeTransform;
    scene->setAttitude(osg::Quat(90, osg::Vec3(1, 0, 0)));

    // scene->addChild(makeAxes());

    osg::ref_ptr<osg::Node> axes = osgDB::readNodeFile("RPAxes.osg");
    scene->addChild(axes.get());

    viewer.setSceneData(scene.get());
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());

    viewer.addEventHandler(new osgViewer::WindowSizeHandler);
    /// Go!
    viewer.realize();

    return viewer.run();
}