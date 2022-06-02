
#pragma once


#include <eglbinding/egl/types.h>

#include <gloperate/base/AbstractGLContextFactory.h>

#include <gloperate-headless/gloperate-headless_api.h>


namespace gloperate_headless
{


/**
*  @brief
*    OpenGL context factory
*/
class GLOPERATE_HEADLESS_API GLContextFactory : public gloperate::AbstractGLContextFactory
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] display
    *    EGL display
    *  @param[in] width
    *    Window width
    *  @param[in] height
    *    Window height
    */
    GLContextFactory(egl::EGLDisplay display, unsigned int width, unsigned int height);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContextFactory();

    // Virtual gloperate::AbstractGLContextFactory functions
    virtual std::unique_ptr<gloperate::AbstractGLContext> createContext(const gloperate::GLContextFormat & format) const override;


private:
    egl::EGLDisplay m_display; ///< EGL display
    unsigned int    m_width;   ///< Window width
    unsigned int    m_height;  ///< Window height
};


} // namespace gloperate_headless
