
# ASSIMP_FOUND
# ASSIMP_INCLUDE_DIRS
# ASSIMP_LIBRARIES
# ASSIMP_BINARY

find_path(ASSIMP_INCLUDE_DIRS assimp/Importer.hpp
    $ENV{ASSIMPDIR}/include
    $ENV{ASSIMP_HOME}/include
    $ENV{PROGRAMFILES}/ASSIMP/include
    /usr/include
    /usr/local/include
    /sw/include
    /opt/local/include
    DOC "The directory where assimp/Importer.hpp etc. resides")

if(MSVC AND X64)
    set(ASSIMP_PF "64")
else()
    set(ASSIMP_PF "86")
endif()

find_library(ASSIMP_LIBRARIES
    NAMES assimp
    PATHS
    $ENV{ASSIMPDIR}/lib/x${ASSIMP_PF}
    $ENV{ASSIMPDIR}/lib${ASSIMP_PF}
    $ENV{ASSIMPDIR}/lib
    $ENV{ASSIMPDIR}/lib/.libs
    $ENV{ASSIMP_HOME}/lib/x${ASSIMP_PF}
    $ENV{ASSIMP_HOME}/lib${ASSIMP_PF}
    $ENV{ASSIMP_HOME}/lib
    $ENV{ASSIMP_HOME}/lib/.libs
    $ENV{ASSIMP_HOME}/build/code
    $ENV{ASSIMP_HOME}/build-debug/code
    $ENV{ASSIMPDIR}
    $ENV{ASSIMP_HOME}
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    DOC "The Assimp library")

if(MSVC)

    find_file(ASSIMP_BINARY
        NAMES assimp.dll "assimp${ASSIMP_PF}.dll"
        PATHS
        $ENV{ASSIMPDIR}/bin/x${ASSIMP_PF}
        $ENV{ASSIMPDIR}/bin${ASSIMP_PF}
        $ENV{ASSIMPDIR}/bin
        $ENV{ASSIMP_HOME}/bin/x${ASSIMP_PF}
        $ENV{ASSIMP_HOME}/bin
        DOC "The Assimp binary")

endif()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ASSIMP REQUIRED_VARS ASSIMP_INCLUDE_DIRS ASSIMP_LIBRARIES)
mark_as_advanced(ASSIMP_INCLUDE_DIRS ASSIMP_LIBRARIES)
