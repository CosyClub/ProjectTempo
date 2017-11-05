# /ext
Contains source code for external libraries used by the project

## FreeImage
The version of FreeImage in use is a fork that comes with a CMake config, found from: https://github.com/rfratto/FreeImage

To compile on windows modification to tif_config.h must be made as per: http://forums.ogre3d.org/viewtopic.php?t=84572
IE: comment out the line "#define snprintf _snprintf" since modern versions of MSVC provide snprintf as part of the stdlib
