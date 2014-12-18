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
#include <osg/Shape>
#include <osg/ShapeDrawable>
#include <osg/Material>
#include <osg/PositionAttitudeTransform>
#include <osg/NodeCallback>

#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>

// Standard includes
#include <iostream>

/// @brief Makes a cylinder along Z
osg::ref_ptr<osg::Geode> makeZCylinder() {
    static const float radius = 0.01;
    static const float height = 0.1;
    osg::ref_ptr<osg::Shape> cyl(
        new osg::Cylinder(osg::Vec3(0, 0, height / 2.0f), radius, height));
    osg::ref_ptr<osg::Drawable> geom = new osg::ShapeDrawable(cyl.get());
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    geode->addDrawable(geom.get());
    return geode;
}

void forceColor(osg::ref_ptr<osg::Node> node, const osg::Vec4 &color) {
    const osg::Vec4 otherColor(0.2, 0.2, 0.2, 1.0); // dark grey for the rest.

    osg::ref_ptr<osg::Material> mat = new osg::Material;
    mat->setColorMode(osg::Material::AMBIENT_AND_DIFFUSE);
    mat->setAmbient(osg::Material::FRONT_AND_BACK, color);
    mat->setDiffuse(osg::Material::FRONT_AND_BACK, otherColor);
    mat->setSpecular(osg::Material::FRONT_AND_BACK, otherColor);
    mat->setShininess(osg::Material::FRONT_AND_BACK,
                      3.0); // only a little shiny
    node->getOrCreateStateSet()->setAttributeAndModes(
        mat.get(), osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

osg::ref_ptr<osg::Group> makeAxes() {
    osg::ref_ptr<osg::Group> ret = new osg::Group;
    osg::ref_ptr<osg::Geode> z = makeZCylinder();
    forceColor(z, osg::Vec4(0, 0, 1, 1));
    ret->addChild(z.get());
    return ret;
}

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