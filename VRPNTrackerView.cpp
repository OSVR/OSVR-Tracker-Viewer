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
#include "VRPNUpdateCallback.h"
#include "OSGQuatlib.h"
#include "VRPNNodeData.h"
#include "VRPNMainloop.h"

// Library/third-party includes
#include <vrpn_Tracker.h>

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
          data(dynamic_cast<VRPNNodeData *>(xform->getUserData())) {}
    osg::MatrixTransform *xform;
    VRPNNodeData *data;
};

inline osg::Matrix toMatrix(vrpn_TRACKERCB const &t) {
    osg::Matrix ret;
    ret.setRotate(toQuat(t.quat));
    ret.setTrans(toVec3(t.pos));
    return ret;
}
void VRPN_CALLBACK poseCallback(void *userdata, const vrpn_TRACKERCB t) {
    CallbackHelper cb(userdata);
    if (cb.data->getSensor() == t.sensor) {
        cb.xform->setMatrix(toMatrix(t));
    }
}

void VRPN_CALLBACK orientationCallback(void *userdata, const vrpn_TRACKERCB t) {
    CallbackHelper cb(userdata);
    if (cb.data->getSensor() == t.sensor) {
        osg::Matrix mat = cb.xform->getMatrix();
        mat.setRotate(toQuat(t.quat));
        cb.xform->setMatrix(mat);
    }
}

class TrackerViewApp {
  public:
    static double worldAxesScale() { return 0.2; }
    static double trackerAxesScale() { return 0.1; }

    TrackerViewApp()
        : m_mainloop(new VRPNMainloop),
          m_scene(new osg::PositionAttitudeTransform),
          m_smallAxes(new osg::MatrixTransform) {

        /// Set the root node's update callback to run the VRPN mainloop
        m_scene->setUpdateCallback(new VRPNUpdateCallback);
        m_scene->setUserData(m_mainloop.get());

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

    void addPoseTracker(std::string const &device, int sensor) {
        m_addTracker(&poseCallback, device, sensor);
    }

    void addOrientationTracker(std::string const &device, int sensor) {
        osg::ref_ptr<osg::MatrixTransform> node =
            m_addTracker(&orientationCallback, device, sensor);

        /// Offset orientation-only trackers up by 1 unit (meter)
        osg::Matrix mat;
        mat.setTrans(0, 0, 1);
        node->setMatrix(mat);
    }

  private:
    template <typename CallbackType>
    osg::ref_ptr<osg::MatrixTransform>
    m_addTracker(CallbackType cb, std::string const &device, int sensor) {
        /// Make scenegraph portion
        osg::ref_ptr<osg::MatrixTransform> node = new osg::MatrixTransform;
        node->addChild(m_smallAxes);
        m_scene->addChild(node);

        /// Create data.
        osg::ref_ptr<VRPNNodeData> data = new VRPNNodeData(device, sensor);
        node->setUserData(data.get());

        /// Set up remote.
        vrpn_Tracker_Remote *tkr = m_mainloop->container().add(
            new vrpn_Tracker_Remote(device.c_str()));
        tkr->register_change_handler(static_cast<void *>(node.get()), cb,
                                     sensor);

        return node;
    }
    osg::ref_ptr<VRPNMainloop> m_mainloop;
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
    args.getApplicationUsage()->setCommandLineUsage(args.getApplicationName() +
                                                    " [options]");
    args.getApplicationUsage()->addCommandLineOption(
        "--pose <device> <sensor>",
        "View a VRPN device's full pose information for the given sensor");
    args.getApplicationUsage()->addCommandLineOption(
        "--ori <device> <sensor>",
        "View a VRPN device's orientation information for the given sensor");

    /// Init the OSG viewer
    osgViewer::Viewer viewer;
    viewer.setUpViewInWindow(20, 20, 640, 480);

    osg::ApplicationUsage::Type helpType;
    if ((helpType = args.readHelpType()) != osg::ApplicationUsage::NO_HELP) {
        args.getApplicationUsage()->write(std::cerr);
        return 1;
    }

    if (args.errors()) {
        args.writeErrorMessages(std::cerr);
        return 1;
    }

    if (args.argc() <= 1) {
        args.getApplicationUsage()->write(
            std::cout, osg::ApplicationUsage::COMMAND_LINE_OPTION);
        return 1;
    }

    TrackerViewApp app;

    std::string device;
    int sensor;
    while (args.read("--pose", device, sensor)) {
        app.addPoseTracker(device, sensor);
    }

    while (args.read("--ori", device, sensor)) {
        app.addOrientationTracker(device, sensor);
    }

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