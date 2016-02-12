
#pragma once


#include <gloperate/base/AbstractGLContext.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class GLContextFormat;


/**
*  @brief
*    Abstract base class for OpenGL context factories
*
*    A context factory is used to create OpenGL contexts.
*    It must be provided by the windowing backend and can
*    be accessed via ViewerContext.
*/
class GLOPERATE_API AbstractGLContextFactory
{
public:
    AbstractGLContextFactory();
    virtual ~AbstractGLContextFactory();

    virtual gloperate::AbstractGLContext * createContext(const gloperate::GLContextFormat & format) = 0;
};


} // namespace gloperate
