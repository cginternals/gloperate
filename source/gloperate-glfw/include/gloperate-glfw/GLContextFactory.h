
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
};


} // namespace gloperate_glfw
