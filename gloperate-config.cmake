
# GLOPERATE_FOUND

# GLOPERATE_LIBRARIES
# GLOPERATE_INCLUDES

# GLOPERATE_LIBRARY
# GLOPERATE_LIBRARY_RELEASE
# GLOPERATE_LIBRARY_DEBUG
# GLOPERATE_INCLUDE_DIR

# GLOPERATE_QT_LIBRARY
# GLOPERATE_QT_LIBRARY_RELEASE
# GLOPERATE_QT_LIBRARY_DEBUG
# GLOPERATE_QT_INCLUDE_DIR

# GLOPERATE_GLFW_LIBRARY
# GLOPERATE_GLFW_LIBRARY_RELEASE
# GLOPERATE_GLFW_LIBRARY_DEBUG
# GLOPERATE_GLFW_INCLUDE_DIR

# GLOPERATE_OSG_LIBRARY
# GLOPERATE_OSG_LIBRARY_RELEASE
# GLOPERATE_OSG_LIBRARY_DEBUG
# GLOPERATE_OSG_INCLUDE_DIR

include(FindPackageHandleStandardArgs)

if(CMAKE_CURRENT_LIST_FILE)
    get_filename_component(GLOPERATE_DIR ${CMAKE_CURRENT_LIST_FILE} PATH)
endif()

file(TO_CMAKE_PATH "$ENV{PROGRAMFILES}" ENVPROGRAMFILES)
file(TO_CMAKE_PATH "$ENV{GLOPERATE_DIR}" ENVGLOPERATE_DIR)

set(GLOPERATE_INCLUDES "")
set(GLOPERATE_LIBRARIES "")

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
        set(${LIB_NAME_UPPER}_LIBRARY "optimized" ${${LIB_NAME_UPPER}_LIBRARY_RELEASE} "debug" ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    elseif(${LIB_NAME_UPPER}_LIBRARY_RELEASE)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_RELEASE})
    elseif(${LIB_NAME_UPPER}_LIBRARY_DEBUG)
        set(${LIB_NAME_UPPER}_LIBRARY ${${LIB_NAME_UPPER}_LIBRARY_DEBUG})
    endif()

    # DEBUG
    # message("${LIB_NAME_UPPER}_INCLUDE_DIR     = ${${LIB_NAME_UPPER}_INCLUDE_DIR}")
    # message("${LIB_NAME_UPPER}_LIBRARY_RELEASE = ${${LIB_NAME_UPPER}_LIBRARY_RELEASE}")
    # message("${LIB_NAME_UPPER}_LIBRARY_DEBUG   = ${${LIB_NAME_UPPER}_LIBRARY_DEBUG}")
    # message("${LIB_NAME_UPPER}_LIBRARY         = ${${LIB_NAME_UPPER}_LIBRARY}")

    list(APPEND GLOPERATE_INCLUDES ${${LIB_NAME_UPPER}_INCLUDE_DIR})
    list(APPEND GLOPERATE_LIBRARIES ${${LIB_NAME_UPPER}_LIBRARY})
endmacro()

find(gloperate gloperate/gloperate_api.h ${LIB_PATHS})
find(gloperate-qt gloperate-qt/gloperate-qt_api.h ${LIB_PATHS})
find(gloperate-glfw gloperate-glfw/gloperate-glfw_api.h ${LIB_PATHS})
find(gloperate-osg gloperate-osg/gloperate-osg_api.h ${LIB_PATHS})

# add dependencies
find_package(globjects REQUIRED)
list(APPEND GLOPERATE_INCLUDES ${GLOBJECTS_INCLUDES})
list(APPEND GLOPERATE_LIBRARIES ${GLOBJECTS_LIBRARIES})

# DEBUG
# message("GLOPERATE_INCLUDES  = ${GLOPERATE_INCLUDES}")
# message("GLOPERATE_LIBRARIES = ${GLOPERATE_LIBRARIES}")

find_package_handle_standard_args(GLOPERATE DEFAULT_MSG GLOPERATE_LIBRARIES GLOPERATE_INCLUDES)
mark_as_advanced(GLOPERATE_FOUND)
