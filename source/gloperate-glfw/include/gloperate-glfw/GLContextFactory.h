
#pragma once


#include <gloperate/base/AbstractGLContextFactory.h>
#include <gloperate/base/AbstractGLContext.h>

#include <gloperate-glfw/gloperate-glfw_api.h>


struct GLFWwindow;

namespace gloperate
{
    class GLContextFormat;
}


namespace gloperate_glfw
{


/**
*  @brief
*    OpenGL context factory
*/
class GLOPERATE_GLFW_API GLContextFactory : public gloperate::AbstractGLContextFactory
{
public:
    GLContextFactory();
    virtual ~GLContextFactory();

    virtual gloperate::AbstractGLContext * createContext(const gloperate::GLContextFormat & format) override;


public:
    // note: requires that glfw::init was previously called 
    static glbinding::Version maxSupportedVersion();

    /**
    *  @brief
    *    Create a hidden window of extend 1x1 with the provided format
    *
    *  @remarks
    *    Requires that glfw::init was previously called
    */
    static GLFWwindow * createWindow(const gloperate::GLContextFormat & format);
};


} // namespace gloperate_glfw
