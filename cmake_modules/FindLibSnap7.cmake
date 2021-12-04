# find libsnap7
#
# exports:
#
#   LibSNAP7_FOUND
#   LibSNAP7_INCLUDE_DIRS
#   LibSNAP7_LIBRARIES
#

find_package(PkgConfig REQUIRED)

# Use pkg-config to get hints about paths
#pkg_check_modules(LibSNAP7_PKGCONF REQUIRED libsnap7)

# Include dir
find_path(LibSNAP7_INCLUDE_DIR
        NAMES snap7.h
        PATHS ${LibSNAP7_PKGCONF_INCLUDE_DIRS}
        )

find_library(
        LibSNAP7_LIBRARY
        NAMES snap7 libsnap7
        PATHS /usr/lib
        #   PATH_SUFFIXES lib
        #   NO_DEFAULT_PATH
)

#FIND_PACKAGE_HANDLE_STANDARD_ARGS(LibSNAP7 DEFAULT_MSG LibSNAP7_LIBRARY LibSNAP7_INCLUDE_DIR)

message("LibSNAP7_INCLUDE_DIR ${LibSNAP7_INCLUDE_DIR}")
message("LibSNAP7_LIBRARY ${LibSNAP7_LIBRARY}")

set(LibSnap7_LIBRARIES ${LibSNAP7_LIBRARY})
set(LibSnap7_INCLUDE_DIRS ${LibSNAP7_INCLUDE_DIR})
set(LibSnap7_FOUND yes)

# Set the include dir variables and the libraries and let libfind_process do the rest.
# NOTE: Singular variables for this library, plural for libraries this this lib depends on.
#set(LibSNAP7_PROCESS_INCLUDES LibSNAP7_INCLUDE_DIR)
#set(LibSNAP7_PROCESS_LIBS LibSNAP7_LIBRARY)