
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


private:
    static glbinding::Version maxSupportedVersion();
};


} // namespace gloperate_glfw
