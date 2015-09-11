# OSVR Tracker Viewer
> Maintained at <https://github.com/OSVR/OSVR-Tracker-Viewer>
>
> For details, see <http://osvr.github.io>
>
> For support, see <http://support.osvr.com>

Utility for viewing tracker (pose, position, orientation) data from the OSVR framework.

You may specify paths on the command line:

- `OSVRTrackerView --pose /me/head`
- `OSVRTrackerView --orientation /me/head`

Multiple paths may also be specified.

If no paths are specified on the command line, it will automatically opens these paths:

- Pose
	- `/me/hands/left`
	- `/me/hands/right`
	- `/me/head`

The large set of axes is the world coordinates in OSVR - the viewer loads in the standard orientation. Each smaller set of axes is a tracker.

As is convention, the X axis is red, Y axis is green, and Z axis is blue (xyz-rgb).

## Dependencies

- OSVR-Core
- OpenSceneGraph: tested with a post-2.8.5 release, and a recent 3.x release, so pretty much any version from the past few years. **Note that you must use a version of OpenSceneGraph built with the same version of Visual Studio you're using** (if you're using Visual Studio) - you can use an OSVR-Core binary snapshot itself with a wide range of different versions, but OSG must match versions.

## License

This project: Licensed under the Apache License, Version 2.0.

