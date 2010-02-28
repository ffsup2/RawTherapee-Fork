cmake_minimum_required(VERSION 2.6)

SET(CMAKE_BUILD_TYPE Debug CACHE STRING "One of: None Debug Release RelWithDebInfo MinSizeRel." FORCE)

option (WITH_RAWZOR "Build with Rawzor support" ON)

add_subdirectory (rtexif)
add_subdirectory (rtengine)
add_subdirectory (rtgui)

