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

#ifndef INCLUDED_OSGMathInterop_h_GUID_D4FC2A1C_6EFF_419D_18FF_EB4C4D3B3066
#define INCLUDED_OSGMathInterop_h_GUID_D4FC2A1C_6EFF_419D_18FF_EB4C4D3B3066

// Internal Includes
// - none

// Library/third-party includes
#include <osvr/Util/Pose3C.h>

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/Matrix>

// Standard includes
// - none

inline osg::Vec3 toVec3(OSVR_Vec3 const &v) {
    return osg::Vec3(v.data[0], v.data[1], v.data[2]);
}

inline osg::Quat toQuat(OSVR_Quaternion const &q) {
    return osg::Quat(osvrQuatGetX(&q), osvrQuatGetY(&q), osvrQuatGetZ(&q),
                     osvrQuatGetW(&q));
}

inline osg::Matrix toMatrix(OSVR_Pose3 const &pose) {
    osg::Matrix ret;
    ret.setRotate(toQuat(pose.rotation));
    ret.setTrans(toVec3(pose.translation));
    return ret;
}

#endif // INCLUDED_OSGMathInterop_h_GUID_D4FC2A1C_6EFF_419D_18FF_EB4C4D3B3066
