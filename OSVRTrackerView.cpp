/** @file
    @brief Implementation

    @date 2014

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2014 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
#include <osg/LineWidth>
#include <osg/Version>

#include <osgDB/ReadFile>

#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>

#include <osgGA/TrackballManipulator>

// Standard includes
#include <iostream>
#include <cmath> // for floor

/// A little utility class to draw a simple grid.
class Grid : public osg::Group {
  public:
    Grid(unsigned int line_count = 49, float line_spacing = 1.0f,
         unsigned int bold_every_n = 0) {
        this->addChild(make_grid(line_count, line_spacing));
#if 0
        std::cout << "Regular: count = " << line_count
                  << ", spacing = " << line_spacing << std::endl;
#endif

        // Bold grid
        if (bold_every_n > 0) {
            line_count = static_cast<unsigned int>(
                             std::floor(line_count / bold_every_n)) +
                         1;
            line_spacing *= bold_every_n;

#if 0
            std::cout << "Bold: count = " << line_count
                      << ", spacing = " << line_spacing << std::endl;
#endif
            osg::MatrixTransform *mt = make_grid(line_count, line_spacing);

            osg::StateSet *stateset = new osg::StateSet();
            osg::LineWidth *linewidth = new osg::LineWidth();
            linewidth->setWidth(2.0f);
            stateset->setAttributeAndModes(linewidth, osg::StateAttribute::ON);
            stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
            mt->setStateSet(stateset);

            this->addChild(mt);
        }

        // Heavy origin lines
        this->addChild(make_axes(line_count, line_spacing));
    }

    osg::MatrixTransform *make_grid(const unsigned int line_count,
                                    const float line_spacing) {
        const unsigned int numVertices = 2 * 2 * line_count;
        osg::Vec3Array *vertices = new osg::Vec3Array(numVertices);
        float length = static_cast<float>(line_count - 1) * line_spacing;
        osg::Vec3Array::size_type ptr = 0;

        for (unsigned int i = 0; i < line_count; ++i) {
            (*vertices)[ptr++].set(-length / 2.0f + i * line_spacing,
                                   length / 2.0f, 0.0f);
            (*vertices)[ptr++].set(-length / 2.0f + i * line_spacing,
                                   -length / 2.0f, 0.0f);
        }

        for (unsigned int i = 0; i < line_count; ++i) {
            (*vertices)[ptr++].set(length / 2.0f,
                                   -length / 2.0f + i * line_spacing, 0.0f);
            (*vertices)[ptr++].set(-length / 2.0f,
                                   -length / 2.0f + i * line_spacing, 0.0f);
        }

        osg::Geometry *geometry = new osg::Geometry;
        geometry->setVertexArray(vertices);
        geometry->addPrimitiveSet(new osg::DrawArrays(
            osg::PrimitiveSet::LINES, 0, static_cast<GLsizei>(numVertices)));

        osg::Geode *geode = new osg::Geode;
        geode->addDrawable(geometry);
        geode->getOrCreateStateSet()->setMode(GL_LIGHTING, 0);

        osg::MatrixTransform *grid_transform = new osg::MatrixTransform;
        grid_transform->setMatrix(osg::Matrix::rotate(osg::PI_2, 1, 0, 0));
        grid_transform->addChild(geode);

        return grid_transform;
    }

    osg::MatrixTransform *make_axes(const unsigned int line_count,
                                    const float line_spacing) {
        const float length = (line_count - 1) * line_spacing;
        const int num_vertices = 6;
        osg::Vec3Array *vertices = new osg::Vec3Array(num_vertices);
        (*vertices)[0].set(-length / 2.0f, 0.0f, 0.0f);
        (*vertices)[1].set(length / 2.0f, 0.0f, 0.0f);
        (*vertices)[2].set(0.0f, -length / 2.0f, 0.0f);
        (*vertices)[3].set(0.0f, length / 2.0f, 0.0f);
        (*vertices)[4].set(0.0f, 0.0f, -length / 2.0f);
        (*vertices)[5].set(0.0f, 0.0f, length / 2.0f);

        osg::Vec4Array *colors = new osg::Vec4Array(num_vertices);
        (*colors)[0].set(1.0, 0.0, 0.0, 1.0);
        (*colors)[1].set(1.0, 0.0, 0.0, 1.0);
        (*colors)[2].set(0.0, 0.0, 1.0, 1.0);
        (*colors)[3].set(0.0, 0.0, 1.0, 1.0);
        (*colors)[4].set(0.0, 1.0, 0.0, 1.0);
        (*colors)[5].set(0.0, 1.0, 0.0, 1.0);

        osg::Geometry *geometry = new osg::Geometry;
        geometry->setVertexArray(vertices);
        geometry->addPrimitiveSet(
            new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, num_vertices));
#if OSG_VERSION_GREATER_THAN(3, 1, 7)
        geometry->setColorArray(colors, osg::Array::BIND_PER_VERTEX);
#else
        geometry->setColorArray(colors);
        geometry->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
#endif

        osg::Geode *geode = new osg::Geode;
        geode->addDrawable(geometry);
        geode->getOrCreateStateSet()->setMode(GL_LIGHTING, 0);

        osg::MatrixTransform *grid_transform = new osg::MatrixTransform;
        grid_transform->setMatrix(osg::Matrix::rotate(osg::PI_2, 1, 0, 0));
        grid_transform->addChild(geode);

        osg::StateSet *stateset = new osg::StateSet();
        osg::LineWidth *linewidth = new osg::LineWidth();
        linewidth->setWidth(4.0f);
        stateset->setAttributeAndModes(linewidth, osg::StateAttribute::ON);
        stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
        grid_transform->setStateSet(stateset);

        return grid_transform;
    }
};

static const char MODEL_FN[] = "RPAxes.osg";

static const char MESSAGE_PREFIX[] = "\n[TrackerViewer] ";

class TrackedSensor : public OSVRInterfaceData {
  public:
    TrackedSensor(OSVRContext &ctx, std::string const &path,
                  osg::MatrixTransform &owningNode, osg::Node &child)
        : OSVRInterfaceData(ctx, path), enabledYet_(false) {

        // Create the switch: child of the transform we'll edit, parent of the
        // child passed to us.
        osg::ref_ptr<osg::Switch> switchNode = new osg::Switch;
        owningNode.addChild(switchNode.get());
        switchNode->addChild(&child);
        // Start with switch off: don't display until we get data.
        switchNode->setAllChildrenOff();

        // Keep a non-owning copy of the pointer for ourselves, now that someone
        // else is holding a reference to that node.
        switchNode_ = switchNode.get();
    }

    /// Register this callback with the userdata being the MatrixTransform
    /// pointer
    static void poseCallback(void *userdata,
                             const OSVR_TimeValue * /*timestamp*/,
                             const OSVR_PoseReport *report) {
        invokeRealCallback(userdata, *report);
    }

    /// Register this callback with the userdata being the MatrixTransform
    /// pointer
    static void orientationCallback(void *userdata,
                                    const OSVR_TimeValue * /*timestamp*/,
                                    const OSVR_OrientationReport *report) {
        invokeRealCallback(userdata, *report);
    }

  private:
    template <typename ReportType>
    static void invokeRealCallback(void *userdata, const ReportType &report) {
        // Cast our userdata - should be the matrixtransform that owns us.
        osg::MatrixTransform &xform =
            *static_cast<osg::MatrixTransform *>(userdata);

        // Now, extract ourselves from that transform
        TrackedSensor &self =
            *dynamic_cast<TrackedSensor *>(xform.getUserData());

        // Handle first-time reports
        if (!self.enabledYet_) {
            std::cout << MESSAGE_PREFIX << self.getPath()
                      << " - got first report, enabling display!\n\n"
                      << std::endl;
            self.enabledYet_ = true;
            self.switchNode_->setAllChildrenOn();
        }
        // Then, proceed to the work of the "real" callback.
        // (Overloaded on report type, so this common code can be shared)
        self.callbackReal(xform, report);
    }
    // Real pose callback
    void callbackReal(osg::MatrixTransform &xform,
                      const OSVR_PoseReport &report) {
        xform.setMatrix(toMatrix(report.pose));
    }
    // Real orientation callback
    void callbackReal(osg::MatrixTransform &xform,
                      const OSVR_OrientationReport &report) {
        osg::Matrix mat = xform.getMatrix();
        mat.setRotate(toQuat(report.rotation));
        xform.setMatrix(mat);
    }
    bool enabledYet_;
    osg::Switch *switchNode_;
};

class TrackerViewApp {
  public:
    static double worldAxesScale() { return 0.2; }
    static double trackerAxesScale() { return 0.1; }

    TrackerViewApp()
        : m_ctx(new OSVRContext(
              "org.osvr.trackerview")) /// Set up OSVR: making an OSG
                                       /// ref-counted object hold
                                       /// the context.
          ,
          m_scene(new osg::PositionAttitudeTransform),
          m_smallAxes(new osg::MatrixTransform), m_numTrackers(0) {

        /// Transform into default OSVR coordinate system: z near.
        m_scene->setAttitude(osg::Quat(90, osg::Vec3(1, 0, 0)));

        /// Set the root node's update callback to run the OSVR update,
        /// and give it the context ref
        m_scene->setUpdateCallback(new OSVRUpdateCallback);
        m_scene->setUserData(m_ctx.get());

        /// Load the basic model for axes
        osg::ref_ptr<osg::Node> axes = osgDB::readNodeFile(MODEL_FN);
        if (!axes) {
            std::cerr << MESSAGE_PREFIX << "Error: Could not read model "
                      << MODEL_FN << std::endl;
            throw std::runtime_error("Could not load model");
        }

        /// Small axes for trackers
        m_smallAxes->setMatrix(osg::Matrixd::scale(
            trackerAxesScale(), trackerAxesScale(), trackerAxesScale()));
        m_smallAxes->addChild(axes.get());

        /// Grid
        m_scene->addChild(new Grid(16, 0.1f, 5));
    }

    osg::ref_ptr<osg::PositionAttitudeTransform> getScene() { return m_scene; }

    void addPoseTracker(std::string const &path) {
        m_addTracker(&TrackedSensor::poseCallback, path);
    }

    void addOrientationTracker(std::string const &path) {
        osg::ref_ptr<osg::MatrixTransform> node =
            m_addTracker(&TrackedSensor::orientationCallback, path);

#if 0
        /// Offset orientation-only trackers up by 1 unit (meter)
        osg::Matrix mat;
        mat.setTrans(0, 1, 0);
        node->setMatrix(mat);
#endif
    }

    int getNumTrackers() const { return m_numTrackers; }

  private:
    template <typename CallbackType>
    osg::ref_ptr<osg::MatrixTransform> m_addTracker(CallbackType cb,
                                                    std::string const &path) {
        /// Make a transform node and attach it to the scene.
        // The tracked sensor class will attach the desired child to the
        // transform node.
        osg::ref_ptr<osg::MatrixTransform> xform = new osg::MatrixTransform;
        m_scene->addChild(xform.get());

        /// Create object holding OSVR interface (which will add m_smallAxes to
        /// the scenegraph) and set callback
        osg::ref_ptr<TrackedSensor> data =
            new TrackedSensor(*m_ctx, path, *xform, *m_smallAxes);
        data->getInterface().registerCallback(cb,
                                              static_cast<void *>(xform.get()));

        /// Transfer ownership of the interface holder/tracked sensor object to
        /// the transform node
        xform->setUserData(data.get());

        m_numTrackers++;
        return xform;
    }
    osg::ref_ptr<OSVRContext> m_ctx;
    osg::ref_ptr<osg::PositionAttitudeTransform> m_scene;
    osg::ref_ptr<osg::MatrixTransform> m_smallAxes;

    int m_numTrackers;
};

int main(int argc, char **argv) {
    /// Parse arguments
    osg::ArgumentParser args(&argc, argv);
    args.getApplicationUsage()->setApplicationName(args.getApplicationName());
    args.getApplicationUsage()->setDescription(
        args.getApplicationName() +
        " is a tool for visualizing tracking data from the OSVR system.");
    args.getApplicationUsage()->setCommandLineUsage(args.getApplicationName() +
                                                    " [options] osvrpath ...");
    args.getApplicationUsage()->addCommandLineOption(
        "--orientation <path>", "add an orientation tracker");
    args.getApplicationUsage()->addCommandLineOption("--pose <path>",
                                                     "add a pose tracker");

    /// Init the OSG viewer
    osgViewer::Viewer viewer(args);
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
    try {
        TrackerViewApp app;

        std::string path;

        // Get pose paths
        while (args.read("--pose", path)) {
            app.addPoseTracker(path);
        }

        // Get orientation paths
        while (args.read("--orientation", path)) {
            app.addOrientationTracker(path);
        }

        // Assume free strings are pose paths
        for (int pos = 1; pos < args.argc(); ++pos) {
            if (args.isOption(pos))
                continue;

            app.addPoseTracker(args[pos]);
        }
        // If no trackers were specified, fall back on these defaults
        if (0 == app.getNumTrackers()) {
            std::cout << "\n" << MESSAGE_PREFIX
                      << "No arguments passed: default is as if you passed the "
                         "following:\n "
                         "--pose /me/hands/left --pose /me/hands/right --pose "
                         "/me/head\n"
                         "You can specify --pose or --orientation then a path, "
                         "as many times as you want.\n"
                         "Pass the argument --help for more info.\n"
                      << std::endl;
            app.addPoseTracker("/me/hands/left");
            app.addPoseTracker("/me/hands/right");
            app.addPoseTracker("/me/head");
        }
        args.reportRemainingOptionsAsUnrecognized();

        if (args.errors()) {
            args.writeErrorMessages(std::cerr);
            return 1;
        }

        viewer.setSceneData(app.getScene());
    } catch (std::exception const &e) {
        std::cerr << MESSAGE_PREFIX << "Exception: " << e.what() << std::endl;
        std::cerr << MESSAGE_PREFIX << "Press enter to exit!" << std::endl;
        std::cin.ignore();
        return -1;
    }

    viewer.setCameraManipulator(new osgGA::TrackballManipulator());

    viewer.addEventHandler(new osgViewer::WindowSizeHandler);
    /// Go!
    viewer.realize();

    return viewer.run();
}
