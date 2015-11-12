# Post-build script for use in CI to create a redistribution bundle

# Copyright 2015 Sensics, Inc.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

$Dest = "install"
$Src = "deps\install"

# OSG Library names to copy - will be prefixed by the soname, etc.
$OSGLibs = 'osg', 'osgDB', 'osgGA', 'osgText', 'osgUtil', 'osgViewer'

# Files to copy from the OSG plugin directory - right now just need to copy enough
# to be able to load the compiled-in .osg-format string.
$OSGPlugins = 'osgdb_deprecated_osg.dll', 'osgdb_osg.dll'

# Files to copy out of the OSVR install tree.
$OSVRFiles = 'osvr-ver.txt',
    'bin\osvrClientKit.dll',
    'bin\osvrClient.dll',
    'bin\osvrCommon.dll',
    'bin\osvrUtil.dll',
    'bin\msvcp120.dll',
    'bin\msvcr120.dll'

# "Entry Point" function
function Main() {
    Write-Host "Copying OpenSceneGraph binaries to install directory"
    Copy-OSG

    Write-Host "Copying OSVR client binaries to install directory"
    Copy-OSVRClientRedist

    # Copy documentation/licenses to redist with binaries
    Write-Host "Copying redist directory contents to install directory"
    Copy-Item "redist\*" -Destination "$Dest"
    
    # Move and zip the results
    $OutName = Get-OutputName
    Write-Host "Moving install directory to '$OutName'"
    Move-Item $Dest $OutName
    Write-Host "Creating compressed output"
    Zip-Output $OutName
    
    Write-Host "ci-postbuild complete!"
}

function Copy-OSG() {
    
    $BinDir = Join-Path "$Src" "bin"

    # Gets the "soname" part of the name - like 'osg100-' - that comes before the library name.
    $OSGPrefix = (Get-Item "$BinDir\osg*-osg.dll").Name -ireplace 'osg.dll$',''

    # Gets the plugin directory.
    $PluginDir = Get-Item "$BinDir\osgPlugins-*"

    ###
    # Compute OSG files to copy
    ###

    # Copies the OpenThreads dll - different soname than OSG, and just one, so no fancy scripting here.
    $OTLib = (Get-Item "$BinDir\ot*-OpenThreads.dll").FullName

    # Get all the OSG libraries to copy
    $OSGLibFullPaths = $OSGLibs | % {Join-Path $BinDir "${OSGPrefix}$_.dll"}

    # Get all the plugins to copy
    $PluginFullPaths = $OSGPlugins | % {Join-Path $PluginDir $_}

    ###
    # Do the Copying!
    ###

    $AllTheThings = @($OTLib) + $OSGLibFullPaths + $PluginFullPaths

    # Copy ALL THE THINGS!
    Copy-Item $AllTheThings -Destination "$Dest"
}


function Copy-OSVRClientRedist() {
    $OSVRPaths = $OSVRFiles| % {Join-Path $Src "$_"}
    Copy-Item $OSVRPaths -Destination "$Dest"
}

function Get-OutputName() {
    $Ver = Get-Content (Join-Path $Src 'osvr-ver.txt')
    $BuildNum = $env:BUILD_NUMBER
    $OutName = "OSVR-Tracker-View-built-with-$Ver-viewer-$BuildNum"
    return $OutName
}
function Zip-Output($OutName) {
    7za a "${OutName}.7z" "${OutName}\*"
}

# call the entry point
Main