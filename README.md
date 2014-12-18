# OSVR Tracker Viewer

Utility for viewing tracker (pose, position, orientation) data from the OSVR framework.

Right now, it just automatically opens these paths:

- Pose
	- `/me/hands/left`
	- `/me/hands/right`
- Orientation (displaced vertically for visibility)
	- `/me/head`

The large set of axes is the world coordinates in OSVR - the viewer loads in the standard orientation. Each smaller set of axes is a tracker.

As is convention, the X axis is red, Y axis is green, and Z axis is blue (xyz-rgb).

## Dependencies

- OSVR-Core
- OpenSceneGraph: tested with a post-2.8.5 release

## License

Copyright 2014 Sensics, Inc. All rights reserved.

(Intended to eventually be released under the Apache License v2.0)