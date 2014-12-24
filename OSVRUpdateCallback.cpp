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
#include "OSVRContext.h"

// Library/third-party includes
#include <osg/Node>
#include <boost/assert.hpp>

// Standard includes
// - none

OSVRUpdateCallback::OSVRUpdateCallback() {}

void OSVRUpdateCallback::operator()(osg::Node *node, osg::NodeVisitor *nv) {
    OSVRContext *ctx = dynamic_cast<OSVRContext *>(node->getUserData());
    BOOST_ASSERT(ctx);
    ctx->update();
    // continue traversal
    traverse(node, nv);
}
OSVRUpdateCallback::~OSVRUpdateCallback() {}

