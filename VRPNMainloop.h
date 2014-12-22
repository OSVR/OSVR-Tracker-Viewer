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

#ifndef INCLUDED_VRPNMainloop_h_GUID_38267139_3369_4D87_91DA_963F51AAE8AE
#define INCLUDED_VRPNMainloop_h_GUID_38267139_3369_4D87_91DA_963F51AAE8AE

// Internal Includes
// - none

// Library/third-party includes
#include <osg/Referenced>
#include <osg/ref_ptr>

#include <vrpn_MainloopContainer.h>

// Standard includes
// - none

/// @brief Class to stash VRPN objects in a node.
class VRPNMainloop : public osg::Referenced {
  public:
    VRPNMainloop() {}

    vrpn_MainloopContainer &container() { return m_container; }

    void mainloop() { m_container.mainloop(); }

  protected:
    virtual ~VRPNMainloop() {}

  private:
    vrpn_MainloopContainer m_container;
};

#endif // INCLUDED_VRPNMainloop_h_GUID_38267139_3369_4D87_91DA_963F51AAE8AE
