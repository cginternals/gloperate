
#pragma once


#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <cppexpose/reflection/Object.h>
#include <cppexpose/signal/Signal.h>

#include <gloperate/base/ChronoTimer.h>


namespace globjects
{
    class Framebuffer;
}


namespace gloperate
{


class Environment;
class AbstractGLContext;
class Renderer;


/**
*  @brief
*    Representation of a canvas onto which can be rendered
*
*    A canvas is attached to a window or offscreen context and handles the
*    actual rendering. It should be embedded by the windowing backend and
*    receives state changes from the outside (such as window size, mouse,
*    or keyboard events) and passes them on to the rendering components.
*/
class GLOPERATE_API Canvas2 : public cppexpose::Object
{
public:
    // Must be emitted only from the UI thread
    cppexpose::Signal<> redraw; ///< Called when the canvas needs to be redrawn
    cppexpose::Signal<> wakeup; ///< Called when the main loop need to wake up


public:
    //@{
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the canvas belongs (must NOT be null!)
    */
    Canvas2(Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Canvas2();
    //@}

    //@{
    /**
    *  @brief
    *    Get gloperate environment
    *
    *  @return
    *    Environment to which the canvas belongs (never null)
    */
    const Environment * environment() const;
    Environment * environment();
    //@}

    //@{
    /**
    *  @brief
    *    Get OpenGL context
    *
    *  @return
    *    OpenGL context used for rendering on the canvas (can be null)
    *
    *  @remarks
    *    The returned context can be null if the canvas has not been
    *    initialized yet, or the method is called between onContextDeinit()
    *    and onContextInit() when the context has been changed.
    *    Aside from that, there should always be a valid OpenGL context
    *    attached to the canvas.
    */
    const AbstractGLContext * openGLContext() const;
    AbstractGLContext * openGLContext();
    //@}

    //@{
    /**
    *  @brief
    *    Set OpenGL context
    *
    *  @param[in] context
    *    OpenGL context used for rendering on the canvas (can be null)
    *
    *  @remarks
    *    This function should only be called by the windowing backend.
    *    If the canvas still has a valid context, onContextDeinit()
    *    will be called and the context pointer will be set to nullptr.
    *    Then, if the new context is valid, the context pointer will be
    *    set to that new context and onContextInit() will be invoked.
    */
    void setOpenGLContext(AbstractGLContext * context);

    // Must be called from render thread
    void render(globjects::Framebuffer * targetFBO);


    virtual void onContextInit();
    virtual void onContextDeinit();

    // Must be called from render thread
    virtual void onRender(globjects::Framebuffer * targetFBO);

    // Must be called from UI thread
    virtual void onUpdateTime();

    // Must be called from UI thread
    virtual void onViewport(
        const glm::vec4 & deviceViewport
      , const glm::vec4 & virtualViewport);

    // Must be called from UI thread
    virtual void onKeyPress(int key, int modifier);
    virtual void onKeyRelease(int key, int modifier);
    virtual void onMouseMove(const glm::ivec2 & pos);
    virtual void onMousePress(int button, const glm::ivec2 & pos);
    virtual void onMouseRelease(int button, const glm::ivec2 & pos);
    virtual void onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos);
    //@}


protected:
    Environment             * m_environment;     ///< Gloperate environment to which the canvas belongs
    AbstractGLContext       * m_openGLContext;   ///< OpenGL context used for rendering onto the canvas
    glm::vec4                 m_deviceViewport;  ///< Viewport (in real device coordinates)
    glm::vec4                 m_virtualViewport; ///< Viewport (in virtual coordinates)
    gloperate::ChronoTimer    m_clock;           ///< Time measurement
    std::unique_ptr<Renderer> m_renderer;        ///< Render that renders into the canvas
};


} // namespace gloperate
