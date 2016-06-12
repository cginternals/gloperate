
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
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] window
    *    GLFW window that contains the context
    */
    GLContext(GLFWwindow * window);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContext();

    /**
    *  @brief
    *    Get GLFW window
    *
    *  @return
    *    GLFW window that contains the context (cannot be null)
    */
    GLFWwindow * window() const;

    // Virtual gloperate::AbstractGLContext functions
    virtual void use() const override;
    virtual void release() const override;


protected:
    GLFWwindow * m_window;  ///< GLFW window that contains the context (cannot be null)
};


} // namespace gloperate_glfw
