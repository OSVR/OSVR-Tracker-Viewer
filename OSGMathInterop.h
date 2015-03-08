/** @file
    @brief Header

    @date 2014

    @author
    Sensics, Inc.
    <http://sensics.com/osvr>
*/

// Copyright 2014 Sensics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

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
    return osg::Vec3(static_cast<float>(v.data[0]), static_cast<float>(v.data[1]), static_cast<float>(v.data[2]));
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
