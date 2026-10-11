# Find libsnap7.
#
# Exports:
#   LibSnap7_FOUND
#   LibSnap7_INCLUDE_DIRS
#   LibSnap7_LIBRARIES
#
# Optional hints:
#   -DLibSnap7_ROOT=<path>
#   -DSNAP7_ROOT=<path>
#   SNAP7_ROOT=<path> in the environment

find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(PC_LibSnap7 QUIET snap7 libsnap7)
endif()

set(_LibSnap7_ROOT_HINTS)
foreach(_LibSnap7_ROOT_VAR LibSnap7_ROOT SNAP7_ROOT)
    if(DEFINED ${_LibSnap7_ROOT_VAR})
        list(APPEND _LibSnap7_ROOT_HINTS "${${_LibSnap7_ROOT_VAR}}")
    endif()
endforeach()
if(DEFINED ENV{SNAP7_ROOT})
    list(APPEND _LibSnap7_ROOT_HINTS "$ENV{SNAP7_ROOT}")
endif()

find_path(
        LibSnap7_INCLUDE_DIR
        NAMES snap7.h
        HINTS
            ${_LibSnap7_ROOT_HINTS}
            ${PC_LibSnap7_INCLUDE_DIRS}
        PATH_SUFFIXES include include/snap7
        DOC "Include directory for Snap7"
)

find_library(
        LibSnap7_LIBRARY
        NAMES snap7 libsnap7
        HINTS
            ${_LibSnap7_ROOT_HINTS}
            ${PC_LibSnap7_LIBRARY_DIRS}
        PATH_SUFFIXES lib lib64
        DOC "Snap7 library"
)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
        LibSnap7
        REQUIRED_VARS LibSnap7_LIBRARY LibSnap7_INCLUDE_DIR
)

if(LibSnap7_FOUND)
    set(LibSnap7_LIBRARIES "${LibSnap7_LIBRARY}")
    set(LibSnap7_INCLUDE_DIRS "${LibSnap7_INCLUDE_DIR}")
endif()

mark_as_advanced(LibSnap7_INCLUDE_DIR LibSnap7_LIBRARY)
