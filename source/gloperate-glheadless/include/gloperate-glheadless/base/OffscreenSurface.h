
#pragma once


#include <memory>

#include <gloperate/base/GLContextFormat.h>

#include <gloperate-glheadless/gloperate-glheadless_api.h>


namespace gloperate_glheadless
{


class GLContext;


/**
*  @brief
*    Offscreen surface that contains an OpenGL context for rendering
*
*  @remarks
*    This base class is for general use of OpenGL and does not depend on
*    using gloperate for rendering. When using gloperate, the derived
*    class RenderWindow should be used.
*/
class GLOPERATE_GLHEADLESS_API OffscreenSurface
{
public:
    /**
    *  @brief
    *    Constructor
    */
    OffscreenSurface();


    /**
    *  @brief
    *    Destructor
    */
    virtual ~OffscreenSurface();


    /**
    *  @brief
    *    Set desired context format for the window
    *
    *  @param[in] format
    *    OpenGL context format
    *
    *  @remarks
    *    If the window has already been created, the context
    *    will not be changed automatically. Call createContext()
    *    to create a context with the new format.
    */
    void setContextFormat(const gloperate::GLContextFormat & format);

    /**
    *  @brief
    *    Create OpenGL context
    *
    *  @remarks
    *    Will create a new OpenGL context with the currently set
    *    context format. If a context was already active, it will
    *    be destroyed, a new context will be set, and the appropriate
    *    context initialization/deinitialization events will be triggered.
    */
    void createContext();

    /**
    *  @brief
    *    Destroy OpenGL context
    *
    *  @remarks
    *    Will destroy the current OpenGL context. If a context was active,
    *    it will be deinitialized first and the destroyed. Afterwards,
    *    the window has no context set until createContext() is called.
    */
    void destroyContext();

    /**
    *  @brief
    *    Get OpenGL context
    *
    *  @return
    *    OpenGL context (can be null)
    */
    GLContext * context() const;

    /**
    *  @brief
    *    Request update (repaint) of the window
    */
    void updateGL();


protected:
    /**
    *  @brief
    *    Initialize OpenGL context
    */
    void initializeContext();

    /**
    *  @brief
    *    De-initialize OpenGL context
    */
    void deinitializeContext();

    /**
    *  @brief
    *    Resize OpenGL scene
    */
    void resize(int width, int height);

    /**
    *  @brief
    *    Render OpenGL scene
    */
    void paint();


protected:
    /**
    *  @brief
    *    Called when a new OpenGL context has been initialized
    */
    virtual void onContextInit();

    /**
    *  @brief
    *    Called when the OpenGL context is about to be deinitialized
    */
    virtual void onContextDeinit();

    /**
    *  @brief
    *    Called whenever the window has been resized
    */
    virtual void onResize(int width, int height);

    /**
    *  @brief
    *    Called to redraw the OpenGL scene
    */
    virtual void onPaint();


protected:
    gloperate::GLContextFormat   m_format;        ///< Desired OpenGL format
    std::unique_ptr<GLContext>   m_context;       ///< Context wrapper for gloperate (can be null)
    bool                         m_initialized;   ///< Has the rendering already been initialized?
};


} // namespace gloperate_glheadless
