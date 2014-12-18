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
#include "OSVRInterfaceData.h"
#include "OSVRContext.h"

// Library/third-party includes
#include <osvr/ClientKit/ClientKit.h>

// Standard includes
// - none

OSVRInterfaceData::OSVRInterfaceData(OSVRContext *ctx, std::string const &path)
    : m_ctx(ctx), m_iface(m_ctx->getContext().getInterface(path)),
      m_path(path) {}

osvr::clientkit::Interface &OSVRInterfaceData::getInterface() {
    return m_iface;
}

std::string const &OSVRInterfaceData::getPath() const { return m_path; }

OSVRInterfaceData::~OSVRInterfaceData() {
    /// Force freeing this interface.
    m_iface.free();
}
