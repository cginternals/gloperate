
#pragma once


#include <gloperate/base/AbstractGLContext.h>


namespace gloperate
{


class GLContextFormat;


/**
*  @brief
*    Abstract base class for OpenGL context factories
*
*    A context factory is used to create OpenGL contexts.
*    It must be provided by the windowing backend and can
*    be accessed via Environment.
*/
class GLOPERATE_API AbstractGLContextFactory
{
public:
    /**
    *  @brief
    *    Constructor
    */
    AbstractGLContextFactory();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractGLContextFactory();

    /**
    *  @brief
    *    Create best possible OpenGL context for a given format
    *
    *  @param[in] format
    *    Desired OpenGL format
    *
    *  @return
    *    OpenGL context
    *
    *  @remarks
    *    This function tries to create a context that fits the desired
    *    format as good as possible. It internally uses createContext()
    *    to create the actual contexts, determine maximum versions etc.
    */
    gloperate::AbstractGLContext * createBestContext(const gloperate::GLContextFormat & format);

    /**
    *  @brief
    *    Create OpenGL context with the given format
    *
    *  @param[in] format
    *    Desired OpenGL format
    *
    *  @return
    *    OpenGL context
    */
    virtual gloperate::AbstractGLContext * createContext(const gloperate::GLContextFormat & format) = 0;
};


} // namespace gloperate
