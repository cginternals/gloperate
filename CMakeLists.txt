
cmake_minimum_required(VERSION 2.8.9 FATAL_ERROR)


# Project description and (meta) information

set(META_PROJECT_NAME        "gloperate")
set(META_PROJECT_DESCRIPTION "C++ library for defining and controlling modern GPU rendering/processing operations")
set(META_VERSION_MAJOR       "0")
set(META_VERSION_MINOR       "1")
set(META_VERSION_PATCH       "0")
set(META_VERSION             "${META_VERSION_MAJOR}.${META_VERSION_MINOR}.${META_VERSION_PATCH}")
set(META_AUTHOR_ORGANIZATION "hpicgs group")
set(META_AUTHOR_DOMAIN       "https://github.com/hpicgs/gloperate/")
set(META_AUTHOR_MAINTAINER   "stefan.buschmann@hpi.de")

string(TOUPPER ${META_PROJECT_NAME} META_PROJECT_NAME_UPPER)

# Limit supported configuration types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)

# Set project name and type (C/C++)
project(${META_PROJECT_NAME} C CXX)


# Configuration options

option(OPTION_PORTABLE_INSTALL  "Install to a local directory instead of the system" OFF)
option(OPTION_BUILD_STATIC      "Build static libraries" OFF)
option(OPTION_BUILD_TESTS       "Build tests (if gmock and gtest are found)" ON)
option(OPTION_BUILD_EXAMPLES    "Build examples (requires optional module glfw and/or Qt5)" OFF)


if(OPTION_BUILD_STATIC)
   set(BUILD_SHARED_LIBS OFF)
   message("Note: ${META_PROJECT_NAME_UPPER}_STATIC needs to be defined for static linking.")
else()
    set(BUILD_SHARED_LIBS ON)
endif()


# CMake configuration

# Include cmake modules from ./cmake
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set configuration types
if(OPTION_LIMIT_CONFIGS)
    set(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Limited Configs" FORCE)
endif()
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build, options are: ${CMAKE_CONFIGURATION_TYPES}")

# Reorder CMAKE_CONFIGURATION_TYPES list so that the chosen CMAKE_BUILD_TYPE is the first element.
if (NOT "${CMAKE_BUILD_TYPE}" STREQUAL "")
    list(REMOVE_ITEM CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}")
    list(INSERT CMAKE_CONFIGURATION_TYPES 0 "${CMAKE_BUILD_TYPE}")
endif()

# Project
project(${META_PROJECT_NAME} C CXX)

# Generate folders for IDE targets (e.g., VisualStudio solutions)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(IDE_FOLDER "")  # Put projects in root folder by default

# Include custom cmake functions
include(cmake/Custom.cmake)
include(cmake/GitRevision.cmake)


# Platform and architecture

# Architecture (32/64 bit)
set(X64 OFF)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(X64 ON)
endif()

# This policy was introduced in 3.0.0 and does not allow for COMPILER_DEFINITIONS_<Config>,
# anymore, but instead requires generator expressions like $<CONFIG:Debug> ... 
# For now the current compile-flag, -definitions, and linker-flags setup shall remain as is.
if(POLICY CMP0043)
    cmake_policy(SET CMP0043 OLD)
endif()

# This policy was introduced in 3.0.0
# Policy CMP0028 is not set: Double colon in target name means ALIAS or IMPORTED target.
if (POLICY CMP0028)
    cmake_policy(SET CMP0028 NEW)
endif()

# Setup platform specifics (compile flags, etc., ...)
if(MSVC)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsMSVC.cmake)
elseif(WIN32 AND CMAKE_COMPILER_IS_GNUCXX)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformWindowsGCC.cmake)
elseif(APPLE)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformMacOS.cmake)
elseif(UNIX AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformLinuxClang.cmake)
elseif(UNIX)
    include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PlatformLinuxGCC.cmake)
else()
    message(WARNING "Unsupported platform/compiler combination")
endif()

# Installation paths
set(project ${META_PROJECT_NAME})
if(WIN32) 
    
    # "%PROGRAMFILES%/<project>/"
    
    set(INSTALL_ROOT          ".")
    set(INSTALL_EXAMPLES      "bin")
    set(INSTALL_DATA          "bin")
    set(INSTALL_BIN           "bin")
    set(INSTALL_PLUGINS       "bin/plugins")
    set(INSTALL_PLUGINS_DEBUG "bin/plugins/debug")
    set(INSTALL_SHARED        ".")
    set(INSTALL_LIB           "lib")
    set(INSTALL_INCLUDE       "include")
    set(INSTALL_DOC           "doc")
    set(INSTALL_SHORTCUTS     ".") # not available in windows
    set(INSTALL_ICONS         ".") # not available in windows
    set(INSTALL_INIT          ".") # not available in windows

else() 
    
    # "/user/[local]/"
    
    set(INSTALL_ROOT          "share/${project}")
    set(INSTALL_EXAMPLES      "share/${project}/examples")
    set(INSTALL_DATA          "share/${project}/examples")
    set(INSTALL_BIN           "bin")
    set(INSTALL_PLUGINS       "share/${project}/examples/plugins")
    set(INSTALL_PLUGINS_DEBUG "share/${project}/examples/plugins/debug")
    set(INSTALL_SHARED        "lib")
    set(INSTALL_LIB           "lib")
    set(INSTALL_INCLUDE       "include")
    set(INSTALL_DOC           "share/doc/${project}")
    set(INSTALL_SHORTCUTS     "share/applications")
    set(INSTALL_ICONS         "share/pixmaps")
    set(INSTALL_INIT          "/etc/init") # /etc/init (upstart init scripts)

    # Adjust target paths for portable installs
    if(OPTION_PORTABLE_INSTALL)
        # Put binaries in root directory and keep data directory name
        set(INSTALL_ROOT ".")                   # /<INSTALL_PREFIX>
        set(INSTALL_DATA ".")                   # /<INSTALL_PREFIX>
        set(INSTALL_BIN ".")                    # /<INSTALL_PREFIX>

        # We have to change the RPATH of binaries to achieve a usable local install.
        # [TODO] For binaries, "$ORIGIN/lib" is right, so that libraries are found in ./lib.
        # However, I have not yet tested what happens when libraries use other libraries.
        # In that case, they might need the rpath $ORIGIN instead ...
        set(CMAKE_SKIP_BUILD_RPATH FALSE)            # Use automatic rpath for build
        set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)    # Use specific rpath for INSTALL
        set(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE) # NO automatic rpath for INSTALL

        # Libraries are relative to binary
        if (APPLE)
            set(CMAKE_INSTALL_RPATH "@executable_path/${INSTALL_LIB}")
        else()
            set(CMAKE_INSTALL_RPATH "$ORIGIN/${INSTALL_LIB}")       
        endif()
    else()
        if (APPLE)
            set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/${INSTALL_LIB}") # Add rpath of project libraries
            set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)                       # Add rpaths of depending libraries
        endif()
    endif()

endif()


# Global deployment

# Add a revision file containing the git-head tag for cpack and install
create_revision_file(${CMAKE_BINARY_DIR}/revision ${INSTALL_ROOT})

# Project meta files
install(FILES gloperate-config.cmake DESTINATION ${INSTALL_ROOT})
install(FILES AUTHORS                DESTINATION ${INSTALL_ROOT})
install(FILES LICENSE                DESTINATION ${INSTALL_ROOT})

# Data files
install(DIRECTORY ${CMAKE_SOURCE_DIR}/data DESTINATION ${INSTALL_DATA})

# Include projects

add_subdirectory(source)
add_subdirectory(docs)
add_subdirectory(packages)
