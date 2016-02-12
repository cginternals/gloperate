
#pragma once


#include <gloperate/base/AbstractGLContext.h>

#include <gloperate-glfw/gloperate-glfw_api.h>


struct GLFWwindow;


namespace gloperate_glfw
{


/**
*  @brief
*    OpenGL context implementation based on GLFW
*/
class GLOPERATE_GLFW_API GLContext : public gloperate::AbstractGLContext
{
public:
    GLContext(GLFWwindow * window);
    virtual ~GLContext();

    virtual void use() const override;
    virtual void release() const override;


protected:
    GLContext();


protected:
    GLFWwindow * m_window;
};


} // namespace gloperate_glfw
