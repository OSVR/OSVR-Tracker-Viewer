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
