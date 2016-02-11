
#pragma once


#include <string>

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
*    OpenGL context implementation based on GLFW
*/
class GLOPERATE_GLFW_API GLContext : public gloperate::AbstractGLContext
{
protected:
    // note: requires that glfw::init was previously called 
    static glbinding::Version maxSupportedVersion();


public:
    /**
    *  @brief
    *    Create a hidden window of extend 1x1 with the provided format
    *
    *  @remarks
    *    Requires that glfw::init was previously called
    */
    static GLFWwindow * createWindow(const gloperate::GLContextFormat & format);


public:
    GLContext(GLFWwindow * window);
    virtual ~GLContext();

    virtual glbinding::ContextHandle handle() const override;

    virtual const gloperate::GLContextFormat & format() const override;

    virtual void use() const override;
    virtual void release() const override;


protected:
    GLContext();


protected:
    mutable gloperate::GLContextFormat * m_format;

    GLFWwindow * m_window;
    glbinding::ContextHandle m_handle;
};


} // namespace gloperate_glfw
