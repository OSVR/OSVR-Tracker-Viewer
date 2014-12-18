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
#include "OSVRUpdateCallback.h"
#include "OSGMathInterop.h"
#include "OSVRInterfaceData.h"
#include "OSVRContext.h"

// Library/third-party includes
#include <osvr/ClientKit/ClientKit.h>

#include <osg/ref_ptr>
#include <osg/PositionAttitudeTransform>
#include <osg/MatrixTransform>
#include <osg/NodeCallback>

#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>

// Standard includes
#include <iostream>

void poseCallback(void *userdata, const OSVR_TimeValue * /*timestamp*/,
                  const OSVR_PoseReport *report) {
    osg::MatrixTransform *xform = static_cast<osg::MatrixTransform *>(userdata);
    OSVRInterfaceData *iface =
        dynamic_cast<OSVRInterfaceData *>(xform->getUserData());
    std::cout << "Got report for " << iface->getPath() << std::endl;
    xform->setMatrix(toMatrix(report->pose));
}

template <typename CallbackType>
inline void assignCallback(osg::ref_ptr<OSVRContext> ctx, CallbackType cb,
                           osg::ref_ptr<osg::MatrixTransform> node,
                           std::string const &path) {
    osg::ref_ptr<OSVRInterfaceData> data = ctx->getInterface(path);
    data->getInterface().registerCallback(cb, static_cast<void *>(node.get()));
    node->setUserData(data.get());
}

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

    /// Set up OSVR: making an OSG ref-counted object hold the context.
    osg::ref_ptr<OSVRContext> ctx =
        new OSVRContext("org.opengoggles.osvrtrackerview");

    osg::ref_ptr<osg::PositionAttitudeTransform> scene =
        new osg::PositionAttitudeTransform;
    /// Transform into default OSVR coordinate system: z near.
    scene->setAttitude(osg::Quat(90, osg::Vec3(1, 0, 0)));

    /// Set the root node's update callback to run the OSVR update,
    /// and give it the context ref
    scene->setUpdateCallback(new OSVRUpdateCallback);
    scene->setUserData(ctx.get());

    osg::ref_ptr<osg::Node> axes = osgDB::readNodeFile("RPAxes.osg");

    /// World axes
    scene->addChild(axes.get());

    osg::ref_ptr<osg::MatrixTransform> smallAxes = new osg::MatrixTransform;
    smallAxes->setMatrix(osg::Matrixd::scale(0.1, 0.1, 0.1));
    smallAxes->addChild(axes.get());

    {
        osg::ref_ptr<osg::MatrixTransform> tracked = new osg::MatrixTransform;
        tracked->addChild(smallAxes.get());
        assignCallback(ctx, &poseCallback, tracked, "/me/hands/left");
        scene->addChild(tracked.get());
    }

    viewer.setSceneData(scene.get());
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());

    viewer.addEventHandler(new osgViewer::WindowSizeHandler);
    /// Go!
    viewer.realize();

    return viewer.run();
}