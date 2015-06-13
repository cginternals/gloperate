
# GLOPERATE_FOUND

# GLOPERATE_LIBRARIES
# GLOPERATE_INCLUDE_DIRS
# GLOPERATE_BINARIES

# GLOPERATE_LIBRARIES
# GLOPERATE_LIBRARY_[RELEASE|DEBUG]
# GLOPERATE_INCLUDE_DIR
# GLOPERATE_BINARIES
# GLOPERATE_BINARY_[RELEASE|DEBUG]

# GLOPERATE_QT_LIBRARIES
# GLOPERATE_QT_LIBRARY_[RELEASE|DEBUG]
# GLOPERATE_QT_INCLUDE_DIR
# GLOPERATE_QT_BINARIES
# GLOPERATE_QT_BINARY_[RELEASE|DEBUG]

# GLOPERATE_QTWIDGETS_LIBRARIES
# GLOPERATE_QTWIDGETS_LIBRARY_[RELEASE|DEBUG]
# GLOPERATE_QTWIDGETS_INCLUDE_DIR
# GLOPERATE_QTWIDGETS_BINARIES
# GLOPERATE_QTWIDGETS_BINARY_[RELEASE|DEBUG]

# GLOPERATE_QTAPPLICATION_LIBRARIES
# GLOPERATE_QTAPPLICATION_LIBRARY_[RELEASE|DEBUG]
# GLOPERATE_QTAPPLICATION_INCLUDE_DIR
# GLOPERATE_QTAPPLICATION_BINARIES
# GLOPERATE_QTAPPLICATION_BINARY_[RELEASE|DEBUG]

# GLOPERATE_GLFW_LIBRARIES
# GLOPERATE_GLFW_LIBRARY_[RELEASE|DEBUG]
# GLOPERATE_GLFW_INCLUDE_DIR
# GLOPERATE_GLFW_BINARIES
# GLOPERATE_GLFW_BINARY_[RELEASE|DEBUG]

# GLOPERATE_OSG_LIBRARIES
# GLOPERATE_OSG_LIBRARY_[RELEASE|DEBUG]
# GLOPERATE_OSG_INCLUDE_DIR
# GLOPERATE_OSG_BINARIES
# GLOPERATE_OSG_BINARY_[RELEASE|DEBUG]

# GLOPERATE_ASSIMP_LIBRARIES
# GLOPERATE_ASSIMP_LIBRARY_[RELEASE|DEBUG]
# GLOPERATE_ASSIMP_INCLUDE_DIR
# GLOPERATE_ASSIMP_BINARIES
# GLOPERATE_ASSIMP_BINARY_[RELEASE|DEBUG]


include(FindPackageHandleStandardArgs)

if(CMAKE_CURRENT_LIST_FILE)
    get_filename_component(GLOPERATE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENVPROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{GLOPERATE_DIR}" ENVGLOPERATE_DIR)

set(GLOPERATE_INCLUDE_DIRS "")
set(GLOPERATE_LIBRARIES "")
set(GLOPERATE_BINARIES "")

set(LIB_PATHS   
    ${GLOPERATE_DIR}/build
    ${GLOPERATE_DIR}/build/Release
    ${GLOPERATE_DIR}/build/Debug
    ${GLOPERATE_DIR}/build-release
    ${GLOPERATE_DIR}/build-debug
    ${ENVGLOPERATE_DIR}/lib
    ${GLOPERATE_DIR}/lib
    ${ENVPROGRAMFILES}/gloperate/lib
    /usr/lib
    /usr/local/lib
    /sw/lib
    /opt/local/lib
    /usr/lib64
    /usr/local/lib64
    /sw/lib64
    /opt/local/lib64
)

macro (find LIB_NAME HEADER)
    set(HINT_PATHS ${ARGN})

    string(TOUPPER ${LIB_NAME} LIB_NAME_UPPER)
    string(REPLACE "-" "_" LIB_NAME_UPPER ${LIB_NAME_UPPER})
    
    find_path(${LIB_NAME_UPPER}_INCLUDE_DIR ${HEADER}
        ${ENVGLOPERATE_DIR}/include
        ${ENVGLOPERATE_DIR}/source/${LIB_NAME}/include
        ${GLOPERATE_DIR}/include
        ${GLOPERATE_DIR}/source/${LIB_NAME}/include
        ${ENVPROGRAMFILES}/gloperate/include
        /usr/include
        /usr/local/include
        /sw/include
        /opt/local/include
        DOC "The directory where ${header} resides")

    find_library(${LIB_NAME_UPPER}_LIBRARY_RELEASE
        NAMES ${LIB_NAME}
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} library")
    find_library(${LIB_NAME_UPPER}_LIBRARY_DEBUG
        NAMES ${LIB_NAME}d
        PATHS ${HINT_PATHS}
        DOC "The ${LIB_NAME} debug library")
    
    if(${LIB_NAME_UPPER}_LIBRARY_RELEASE AND ${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARIES "optimized" ${${LIB_NAME_UPPER}_LIBRARY_RELEASE} "debug" ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    elseif(${LIB_NAME_UPPER}_LIBRARY_RELEASE)
        set(${LIB_NAME_UPPER}_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARY_RELEASE})
    elseif(${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    endif()

    # DEBUG
    # message("${LIB_NAME_UPPER}_INCLUDE_DIR     = ${${LIB_NAME_UPPER}_INCLUDE_DIR}")
    # message("${LIB_NAME_UPPER}_LIBRARY_RELEASE = ${${LIB_NAME_UPPER}_LIBRARY_RELEASE}")
    # message("${LIB_NAME_UPPER}_LIBRARY_DEBUG   = ${${LIB_NAME_UPPER}_LIBRARY_DEBUG}")
    # message("${LIB_NAME_UPPER}_LIBRARY         = ${${LIB_NAME_UPPER}_LIBRARY}")

    if (${LIB_NAME_UPPER}_LIBRARIES)
        list(APPEND GLOPERATE_INCLUDE_DIRS ${${LIB_NAME_UPPER}_INCLUDE_DIR})
        list(APPEND GLOPERATE_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARIES})
    endif ()

    # find binaries
    if (WIN32 AND ${LIB_NAME_UPPER}_LIBRARIES)
        set(${LIB_NAME_UPPER}_BINARIES "")

        find_file(${LIB_NAME_UPPER}_BINARY_RELEASE
            NAMES ${LIB_NAME}.dll
            PATHS
            ${GLOPERATE_DIR}/bin
            ${GLOPERATE_DIR}/build/Release
            ${GLOPERATE_DIR}/build-release
            DOC "The ${LIB_NAME_UPPER} binary")

        find_file(${LIB_NAME_UPPER}_BINARY_DEBUG
            NAMES ${LIB_NAME}d.dll
            PATHS
            ${GLOPERATE_DIR}/bin
            ${GLOPERATE_DIR}/build/Debug
            ${GLOPERATE_DIR}/build-debug
            DOC "The ${LIB_NAME_UPPER} debug binary")

        if(NOT ${LIB_NAME_UPPER}_BINARY_RELEASE STREQUAL "${LIB_NAME_UPPER}_BINARY_RELEASE-NOTFOUND")
            list(APPEND ${LIB_NAME_UPPER}_BINARIES ${${LIB_NAME_UPPER}_BINARY_RELEASE})
        endif()

        if(NOT ${LIB_NAME_UPPER}_BINARY_DEBUG STREQUAL "${LIB_NAME_UPPER}_BINARY_DEBUG-NOTFOUND")
            list(APPEND ${LIB_NAME_UPPER}_BINARIES ${${LIB_NAME_UPPER}_BINARY_DEBUG})
        endif()

        if (${LIB_NAME_UPPER}_BINARIES)
            list(APPEND GLOPERATE_BINARIES ${${LIB_NAME_UPPER}_BINARIES})
        endif()
    endif()
endmacro()

find(gloperate gloperate/gloperate_api.h ${LIB_PATHS})
find(gloperate-qt gloperate-qt/gloperate-qt_api.h ${LIB_PATHS})
find(gloperate-qtwidgets gloperate-qtwidgets/gloperate-qtwidgets_api.h ${LIB_PATHS})
find(gloperate-qtapplication gloperate-qtapplication/gloperate-qtapplication_api.h ${LIB_PATHS})
find(gloperate-glfw gloperate-glfw/gloperate-glfw_api.h ${LIB_PATHS})
find(gloperate-osg gloperate-osg/gloperate-osg_api.h ${LIB_PATHS})
find(gloperate-assimp gloperate-assimp/gloperate-assimp_api.h ${LIB_PATHS})

# DEBUG
# message("GLOPERATE_INCLUDE_DIRS  = ${GLOPERATE_INCLUDE_DIRS}")
# message("GLOPERATE_LIBRARIES = ${GLOPERATE_LIBRARIES}")

find_package_handle_standard_args(GLOPERATE DEFAULT_MSG GLOPERATE_LIBRARIES GLOPERATE_INCLUDE_DIRS)
mark_as_advanced(GLOPERATE_FOUND)
