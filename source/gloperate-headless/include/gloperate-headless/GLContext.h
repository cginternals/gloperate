
#pragma once


#include <gloperate/base/AbstractGLContext.h>

#include <gloperate-headless/gloperate-headless_api.h>


namespace egl
{


using EGLDisplay = void*;
using EGLSurface = void*;
using EGLContext = void*;


} // namespace egl


namespace gloperate_headless
{


/**
*  @brief
*    OpenGL context implementation based on eglbinding
*/
class GLOPERATE_HEADLESS_API GLContext : public gloperate::AbstractGLContext
{
public:
    /**
    *  @brief
    *    Update swap behavior
    *
    *  @param[in] swapBehavior
    *    The new behavior for swapping the buffers
    *
    *  @remarks
    *    Operates on the current OpenGL context
    */
    static void updateSwapBehavior(gloperate::GLContextFormat::SwapBehavior swapBehavior);


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] display
    *    EGL display that contains the surface
    *  @param[in] surface
    *    EGL surface that contains the context
    *  @param[in] context
    *    EGL context
    */
    GLContext(egl::EGLDisplay display, egl::EGLSurface surface, egl::EGLContext context);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~GLContext();

    /**
    *  @brief
    *    Get EGL surface
    *
    *  @return
    *    EGL surface that is associated with the context (cannot be null)
    */
    egl::EGLSurface surface() const;

    /**
    *  @brief
    *    Get EGL context
    *
    *  @return
    *    EGL context (cannot be null)
    */
    egl::EGLContext context() const;

    // Virtual gloperate::AbstractContext functions
    virtual void use() const override;
    virtual void release() const override;


protected:
    egl::EGLDisplay m_display; ///< EGL display
    egl::EGLSurface m_surface; ///< EGL surface that contains the context (cannot be null)
    egl::EGLContext m_context; ///< EGL context
};


} // namespace gloperate_headless
