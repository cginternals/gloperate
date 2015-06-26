
#pragma once


#include <string>

#include <gloperate/painter/AbstractContext.h>

#include <gloperate-glfw/gloperate-glfw_api.h>


struct GLFWwindow;


namespace gloperate
{
    class ContextFormat;
}


namespace gloperate_glfw
{


class GLOPERATE_GLFW_API Context : public gloperate::AbstractContext
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

    virtual glbinding::ContextHandle handle() const;

    virtual const gloperate::ContextFormat & format() const;

    virtual void makeCurrent() const;
    virtual void doneCurrent() const;


protected:
    Context();


protected:
    mutable gloperate::ContextFormat * m_format;

    GLFWwindow * m_window;
    glbinding::ContextHandle m_handle;
};


} // namespace gloperate_glfw
