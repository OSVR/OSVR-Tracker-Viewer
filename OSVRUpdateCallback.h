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

#ifndef INCLUDED_OSVRUpdateCallback_h_GUID_E58659E1_0843_429C_C9C9_B75B49A225FE
#define INCLUDED_OSVRUpdateCallback_h_GUID_E58659E1_0843_429C_C9C9_B75B49A225FE

// Internal Includes
// - none

// Library/third-party includes
#include <osvr/ClientKit/Context_decl.h>

#include <osg/NodeCallback>

// Standard includes
// - none

class OSVRUpdateCallback : public osg::NodeCallback {
  public:
    OSVRUpdateCallback();
    virtual ~OSVRUpdateCallback();
    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);
};

#endif // INCLUDED_OSVRUpdateCallback_h_GUID_E58659E1_0843_429C_C9C9_B75B49A225FE
