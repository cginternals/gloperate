
#pragma once


#include <string>

#include <glbinding/ContextHandle.h>
#include <glbinding/Version.h>

#include <gloperate/base/GLContextFormat.h>


namespace gloperate
{


/**
*  @brief
*    Static class for working with the OpenGL context
*
*    This class contains utility functions for working with OpenGL contexts.
*    Most of the functions require a valid OpenGL context to be current at
*    the time of the function call and will, e.g., query the status of the
*    current context. Therefore, the functions are independent from how a
*    context is created or managed (e.g., by Qt, GLFW, or any other method).
*/
class GLOPERATE_API GLContextUtils
{
public:
    /**
    *  @brief
    *    Check if current context is valid
    *
    *  @return
    *    'true' if handle() > 0, else 'false'
    */
    static bool isValid();

    /**
    *  @brief
    *    Try to fetch the handle of the current context
    *
    *  @return
    *    Context handle
    *
    *  @notes
    *    - Requires active context
    */
    static glbinding::ContextHandle tryFetchHandle();

    /**
    *  @brief
    *    Read context format from current context
    *
    *  @return
    *    OpenGL context format
    *
    *  @notes
    *    - Requires active context
    */
    static gloperate::GLContextFormat retrieveFormat();

    /**
    *  @brief
    *    Read OpenGL version from current context
    *
    *  @notes
    *    - Requires active context
    */
    static glbinding::Version retrieveVersion();

    /**
    *  @brief
    *    Get OpenGL version
    *
    *  @return
    *    OpenGL version string
    *
    *  @notes
    *    - Requires active context
    */
    static std::string version();

    /**
    *  @brief
    *    Get OpenGL vendor
    *
    *  @return
    *    OpenGL vendor string
    *
    *  @notes
    *    - Requires active context
    */
    static std::string vendor();

    /**
    *  @brief
    *    Get OpenGL renderer
    *
    *  @return
    *    OpenGL renderer string
    *
    *  @notes
    *    - Requires active context
    */
    static std::string renderer();
};


} // namespace gloperate
