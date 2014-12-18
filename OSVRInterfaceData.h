/** @file
    @brief Header

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

#ifndef INCLUDED_OSVRInterfaceData_h_GUID_20F69D2D_747D_4D90_688E_B5F286B788ED
#define INCLUDED_OSVRInterfaceData_h_GUID_20F69D2D_747D_4D90_688E_B5F286B788ED

// Internal Includes
// - none

// Library/third-party includes
#include <osg/Referenced>
#include <osg/ref_ptr>

#include <osvr/ClientKit/Interface_decl.h>

// Standard includes
#include <string>

class OSVRContext;

/// @brief Class to track the lifetime of a node we're updating with a callback,
/// so we can free the interface (and the callback) when the node is destroyed.
class OSVRInterfaceData : public osg::Referenced {
  public:
    OSVRInterfaceData(OSVRContext *ctx, std::string const &path);
    osvr::clientkit::Interface &getInterface();

    std::string const &getPath() const;

  protected:
    virtual ~OSVRInterfaceData();

  private:
    /// Holding this ensures the context outlives us.
    osg::ref_ptr<OSVRContext> m_ctx;
    osvr::clientkit::Interface m_iface;
    std::string m_path;
};

#endif // INCLUDED_OSVRInterfaceData_h_GUID_20F69D2D_747D_4D90_688E_B5F286B788ED
