
#pragma once


#include <string>

#include <gloperate/base/OpenGLContext.h>

#include <gloperate-glfw/gloperate-glfw_api.h>


struct GLFWwindow;


namespace gloperate
{
    class ContextFormat;
}


namespace gloperate_glfw
{


/**
*  @brief
*    OpenGL context implementation based on GLFW
*/
class GLOPERATE_GLFW_API Context : public gloperate::OpenGLContext
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
    static GLFWwindow * create(const gloperate::ContextFormat & format);


public:
    Context(GLFWwindow * window);
    virtual ~Context();

    virtual glbinding::ContextHandle handle() const override;

    virtual const gloperate::ContextFormat & format() const override;

    virtual void use() const override;
    virtual void release() const override;


protected:
    Context();


protected:
    mutable gloperate::ContextFormat * m_format;

    GLFWwindow * m_window;
    glbinding::ContextHandle m_handle;
};


} // namespace gloperate_glfw
