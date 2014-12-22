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

#ifndef INCLUDED_VRPNUpdateCallback_h_GUID_365D08CA_0F24_4B81_D344_802E7755C4F6
#define INCLUDED_VRPNUpdateCallback_h_GUID_365D08CA_0F24_4B81_D344_802E7755C4F6

// Internal Includes
// - none

// Library/third-party includes
#include <osg/NodeCallback>

// Standard includes
// - none

class VRPNUpdateCallback : public osg::NodeCallback {
  public:
    VRPNUpdateCallback();
    virtual ~VRPNUpdateCallback();
    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv);
};

#endif // INCLUDED_VRPNUpdateCallback_h_GUID_365D08CA_0F24_4B81_D344_802E7755C4F6
