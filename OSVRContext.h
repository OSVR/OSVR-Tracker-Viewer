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

#ifndef INCLUDED_OSVRContext_h_GUID_D82A13C3_E032_4E9A_24FA_1B4AF13B9A9F
#define INCLUDED_OSVRContext_h_GUID_D82A13C3_E032_4E9A_24FA_1B4AF13B9A9F

// Internal Includes
// - none

// Library/third-party includes
#include <osg/Referenced>
#include <osg/ref_ptr>

#include <osvr/ClientKit/Context_decl.h>

// Standard includes
// - none

class OSVRInterfaceData;

/// @brief Class to attach the lifetime of the context to the lifetime of a
/// node.
class OSVRContext : public osg::Referenced {
  public:
    OSVRContext(std::string const &appId);

    osg::ref_ptr<OSVRInterfaceData> getInterface(std::string const &path);
    osvr::clientkit::ClientContext &getContext();
    void update();

  protected:
    virtual ~OSVRContext();

  private:
    osvr::clientkit::ClientContext m_ctx;
};

#endif // INCLUDED_OSVRContext_h_GUID_D82A13C3_E032_4E9A_24FA_1B4AF13B9A9F
