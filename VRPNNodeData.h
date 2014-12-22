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

#ifndef INCLUDED_VRPNNodeData_h_GUID_458B8D6D_93D5_4194_F495_3C1A09F942D4
#define INCLUDED_VRPNNodeData_h_GUID_458B8D6D_93D5_4194_F495_3C1A09F942D4

// Internal Includes
// - none

// Library/third-party includes
#include <osg/Referenced>
#include <osg/ref_ptr>

// Standard includes
// - none

class VRPNNodeData : public osg::Referenced {
  public:
    VRPNNodeData(std::string const &device, int sensor)
        : m_device(device), m_sensor(sensor) {}

    std::string const &getDevice() const { return m_device; }

    int getSensor() const { return m_sensor; }

  protected:
    virtual ~VRPNNodeData() {}

  private:
    std::string m_device;
    int m_sensor;
};

#endif // INCLUDED_VRPNNodeData_h_GUID_458B8D6D_93D5_4194_F495_3C1A09F942D4
