
# GLFW_FOUND
# GLFW_INCLUDE_DIR
# GLFW_LIBRARY

# GLFW_BINARY (win32 only)


find_path(GLFW_INCLUDE_DIR GLFW/glfw3.h

    PATHS
    $ENV{GLFW_DIR}
    /usr
    /usr/local
    /usr/include/GL
    /sw
    /opt/local
    /opt/graphics/OpenGL
    /opt/graphics/OpenGL/contrib/libglfw

    PATH_SUFFIXES
    /include

    DOC "The directory where GLFW/glfw.h resides")

find_library(GLFW_LIBRARY_RELEASE NAMES glfw3 glfw glfw3dll glfwdll

    PATHS
    $ENV{GLFW_DIR}
    /lib/x64
    /lib/cocoa
    /usr
    /usr/local
    /sw
    /opt/local

    # authors prefered choice for development
    /build
    /build-release
    $ENV{GLFW_DIR}/build
    $ENV{GLFW_DIR}/build-release

    PATH_SUFFIXES
    /lib
    /lib64
    /src # for from-source builds

    DOC "The GLFW library")

find_library(GLFW_LIBRARY_DEBUG NAMES glfw3d glfwd glfw3ddll glfwddll

    PATHS
    $ENV{GLFW_DIR}
    /lib/x64
    /lib/cocoa
    /usr
    /usr/local
    /sw
    /opt/local

    # authors prefered choice for development
    /build
    /build-debug
    $ENV{GLFW_DIR}/build
    $ENV{GLFW_DIR}/build-debug

    PATH_SUFFIXES
    /lib
    /lib64
    /src # for from-source builds

    DOC "The GLFW library")

set(GLFW_LIBRARIES "")
if(GLFW_LIBRARY_RELEASE AND GLFW_LIBRARY_DEBUG)
    set(GLFW_LIBRARIES 
        optimized   ${GLFW_LIBRARY_RELEASE}
        debug       ${GLFW_LIBRARY_DEBUG})
elseif(GLFW_LIBRARY_RELEASE)
    set(GLFW_LIBRARIES ${GLFW_LIBRARY_RELEASE})
elseif(GLFW_LIBRARY_DEBUG)
    set(GLFW_LIBRARIES ${GLFW_LIBRARY_DEBUG})
endif()

if(WIN32)

    find_file(GLFW_BINARY glfw3.dll

        HINTS
        ${GLFW_INCLUDE_DIR}/..

        PATHS
        $ENV{GLFW_DIR}
        /lib/x64
        /lib/cocoa

        PATH_SUFFIXES
        /lib
        /bin

        DOC "The GLFW binary")

endif()

if(APPLE)
    set(GLFW_cocoa_LIBRARY "-framework Cocoa" CACHE STRING "Cocoa framework for OSX")
    set(GLFW_iokit_LIBRARY "-framework IOKit" CACHE STRING "IOKit framework for OSX")
endif()

# GLFW is required to link statically for now (no deploy specified)

find_package_handle_standard_args(GLFW DEFAULT_MSG GLFW_LIBRARIES GLFW_INCLUDE_DIR)
mark_as_advanced(GLFW_FOUND GLFW_INCLUDE_DIR GLFW_LIBRARIES)
