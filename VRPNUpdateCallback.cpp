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
#include "VRPNMainloop.h"

// Library/third-party includes
#include <osg/Node>
#include <boost/assert.hpp>

// Standard includes
// - none

VRPNUpdateCallback::VRPNUpdateCallback() {}

void VRPNUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv) {
    VRPNMainloop *mainloop = dynamic_cast<VRPNMainloop *>(node->getUserData());
    BOOST_ASSERT(mainloop);
    mainloop->mainloop();
    // continue traversal
    traverse(node, nv);
}
VRPNUpdateCallback::~VRPNUpdateCallback() {}