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

/// @brief A struct that does our casting for us.
struct CallbackHelper {
    CallbackHelper(void *userdata)
        : xform(static_cast<osg::MatrixTransform *>(userdata)),
          iface(dynamic_cast<OSVRInterfaceData *>(xform->getUserData())) {}
    osg::MatrixTransform *xform;
    OSVRInterfaceData *iface;
};

void poseCallback(void *userdata, const OSVR_TimeValue * /*timestamp*/,
                  const OSVR_PoseReport *report) {
    CallbackHelper cb(userdata);
    cb.xform->setMatrix(toMatrix(report->pose));

    // std::cout << "Got report for " << cb.iface->getPath() << std::endl;
}

class TrackerViewApp {
  public:
    static double worldAxesScale() { return 0.2; }
    static double trackerAxesScale() { return 0.1; }

    TrackerViewApp()
        : m_ctx(new OSVRContext(
              "org.opengoggles.osvrtrackerview")) /// Set up OSVR: making an OSG
                                                  /// ref-counted object hold
                                                  /// the context.
          ,
          m_scene(new osg::PositionAttitudeTransform),
          m_smallAxes(new osg::MatrixTransform) {

        /// Transform into default OSVR coordinate system: z near.
        m_scene->setAttitude(osg::Quat(90, osg::Vec3(1, 0, 0)));

        /// Set the root node's update callback to run the OSVR update,
        /// and give it the context ref
        m_scene->setUpdateCallback(new OSVRUpdateCallback);
        m_scene->setUserData(m_ctx.get());

        /// Load the basic model for axes
        osg::ref_ptr<osg::Node> axes = osgDB::readNodeFile("RPAxes.osg");

        {
            /// World axes
            osg::ref_ptr<osg::MatrixTransform> worldAxes =
                new osg::MatrixTransform;
            worldAxes->setMatrix(osg::Matrixd::scale(
                worldAxesScale(), worldAxesScale(), worldAxesScale()));
            worldAxes->addChild(axes);
            m_scene->addChild(worldAxes.get());
        }

        /// Small axes for trackers
        m_smallAxes->setMatrix(osg::Matrixd::scale(
            trackerAxesScale(), trackerAxesScale(), trackerAxesScale()));
        m_smallAxes->addChild(axes.get());
    }

    osg::ref_ptr<osg::PositionAttitudeTransform> getScene() { return m_scene; }

    void addPoseTracker(std::string const &path) {
        m_addTracker(&poseCallback, path);
    }

  private:
    template <typename CallbackType>
    osg::ref_ptr<osg::MatrixTransform> m_addTracker(CallbackType cb,
                                                    std::string const &path) {
        /// Make scenegraph portion
        osg::ref_ptr<osg::MatrixTransform> node = new osg::MatrixTransform;
        node->addChild(m_smallAxes);
        m_scene->addChild(node);

        /// Get OSVR interface and set callback
        osg::ref_ptr<OSVRInterfaceData> data = m_ctx->getInterface(path);
        data->getInterface().registerCallback(cb,
                                              static_cast<void *>(node.get()));

        /// Transfer ownership of the interface holder to the node
        node->setUserData(data.get());

        return node;
    }
    osg::ref_ptr<OSVRContext> m_ctx;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_scene;
    osg::ref_ptr<osg::MatrixTransform> m_smallAxes;
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

    TrackerViewApp app;
    app.addPoseTracker("/me/hands/left");

    args.reportRemainingOptionsAsUnrecognized();

    if (args.errors()) {
        args.writeErrorMessages(std::cerr);
        return 1;
    }

    viewer.setSceneData(app.getScene());
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());

    viewer.addEventHandler(new osgViewer::WindowSizeHandler);
    /// Go!
    viewer.realize();

    return viewer.run();
}