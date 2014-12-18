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
#include "OSVRContext.h"
#include "OSVRInterfaceData.h"

// Library/third-party includes
#include <osvr/ClientKit/Context.h>

// Standard includes
// - none

OSVRContext::OSVRContext(std::string const &appId) : m_ctx(appId.c_str()) {}

osg::ref_ptr<OSVRInterfaceData>
OSVRContext::getInterface(std::string const &path) {
    osg::ref_ptr<OSVRInterfaceData> data = new OSVRInterfaceData(this, path);
    return data;
}

osvr::clientkit::ClientContext &OSVRContext::getContext() { return m_ctx; }

void OSVRContext::update() { m_ctx.update(); }

OSVRContext::~OSVRContext() {}