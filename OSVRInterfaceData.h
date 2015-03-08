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
