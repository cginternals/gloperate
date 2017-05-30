
#pragma once


#include <gloperate/base/AbstractGLContextFactory.h>

#include <gloperate-glfw/gloperate-glfw_api.h>


struct GLFWmonitor;


namespace gloperate_glfw
{


/**
*  @brief
*    OpenGL context factory
*/
class GLOPERATE_GLFW_API GLContextFactory : public gloperate::AbstractGLContextFactory
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] monitor
    *    GLFW monitor (if valid, fullscreen mode is used, else windowed mode)
    *  @param[in] width
    *    Window width
    *  @param[in] height
    *    Window height
    */
    GLContextFactory(GLFWmonitor * monitor, unsigned int width, unsigned int height);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContextFactory();

    // Virtual gloperate::AbstractGLContextFactory functions
    virtual std::unique_ptr<gloperate::AbstractGLContext> createContext(const gloperate::GLContextFormat & format) const override;


private:
    /**
    *  @brief
    *    Set GLFW states and hints to select the given OpenGL context format
    *
    *  @param[in] format
    *    OpenGL context format
    */
    static void initializeGLFWState(const gloperate::GLContextFormat & format);


private:
    GLFWmonitor * m_monitor; ///< GLFW monitor (if valid, fullscreen mode is used, else windowed mode)
    unsigned int  m_width;   ///< Window width
    unsigned int  m_height;  ///< Window height
};


} // namespace gloperate_glfw
