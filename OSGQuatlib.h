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

#ifndef INCLUDED_OSGQuatlib_h_GUID_843B07DD_CFD8_4A81_48EA_A067D168DACA
#define INCLUDED_OSGQuatlib_h_GUID_843B07DD_CFD8_4A81_48EA_A067D168DACA

// Internal Includes
// - none

// Library/third-party includes
#include <quat.h>

#include <osg/Vec3>
#include <osg/Quat>
#include <osg/Matrix>

// Standard includes
// - none

inline osg::Vec3 toVec3(q_vec_type const v) {
    return osg::Vec3(v[0], v[1], v[2]);
}

inline osg::Quat toQuat(q_type const q) {
    return osg::Quat(q[Q_X], q[Q_Y], q[Q_Z], q[Q_W]);
}

inline osg::Matrix toMatrix(q_xyz_quat_type const &pose) {
    osg::Matrix ret;
    ret.setRotate(toQuat(pose.quat));
    ret.setTrans(toVec3(pose.xyz));
    return ret;
}

#endif // INCLUDED_OSGQuatlib_h_GUID_843B07DD_CFD8_4A81_48EA_A067D168DACA
