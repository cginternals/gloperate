
#pragma once


#include <gloperate/base/AbstractGLContextFactory.h>

#include <gloperate-glfw/gloperate-glfw_api.h>


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
    */
    GLContextFactory();

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContextFactory();

    // Virtual gloperate::AbstractGLContextFactory functions
    virtual gloperate::AbstractGLContext * basicCreateContext(const gloperate::GLContextFormat & format) const override;


private:
    static void initializeGLFWState(const gloperate::GLContextFormat & format);
};


} // namespace gloperate_glfw
