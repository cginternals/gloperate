
#pragma once


#include <glm/fwd.hpp>

#include <cppexpose/reflection/Object.h>
#include <cppexpose/signal/Signal.h>

#include <gloperate/gloperate_api.h>


namespace globjects
{
    class Framebuffer;
}


namespace gloperate
{


class Environment;
class AbstractGLContext;


/**
*  @brief
*    Representation of a canvas onto which can be rendered
*
*    A canvas is attached to a window or offscreen context and handles the
*    actual rendering. It should be embedded by the windowing backend and
*    receives state changes from the outside (such as window size, mouse, or
*    keyboard events) and passes them on to the rendering components.
*/
class GLOPERATE_API Canvas2 : public cppexpose::Object
{
public:
    // Must be emitted only from the UI thread
    cppexpose::Signal<> redraw; ///< Called when the canvas needs to be redrawn
    cppexpose::Signal<> wakeup; ///< Called when the main loop need to wake up


public:
    Canvas2(Environment * environment);
    virtual ~Canvas2();

    const Environment * environment() const;
    Environment * environment();

    // [TODO] Needed? I think we don't need to support changing OpenGL contexts. So, the viewer is supposed
    //        no create a valid context prior to creating any canvases, then create the canvas. Do we need
    //        a pointer to the context then? If so, maybe it should be passed in the constructor?
    const AbstractGLContext * openGLContext() const;
    AbstractGLContext * openGLContext();
    void setOpenGLContext(AbstractGLContext * context);

    // Must be called from render thread
    void render(globjects::Framebuffer * targetFBO);

    // Must be called from render thread
    virtual void onRender(globjects::Framebuffer * targetFBO);

    // Must be called from UI thread
    // [TODO] Needed any more?
    virtual void onUpdate();

    // [TODO] Needed any more?
    virtual void onContextInit();
    virtual void onContextDeinit();

    // Must be called from UI thread
    virtual void onViewport(
        const glm::vec4 & deviceViewport
      , const glm::vec4 & virtualViewport);

    // [TODO] This was supposed to be used by the viewer in order to provide the rendering with
    //        the main background color of the viewer, which might change with different styles.
    //        Do we want this or shall it be gone? I guess most pipelines ignore it anyway.
    virtual void onBackgroundColor(float red, float green, float blue);

    // Must be called from UI thread
    virtual void onKeyPress(int key, int modifier);
    virtual void onKeyRelease(int key, int modifier);
    virtual void onMouseMove(const glm::ivec2 & pos);
    virtual void onMousePress(int button, const glm::ivec2 & pos);
    virtual void onMouseRelease(int button, const glm::ivec2 & pos);
    virtual void onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos);


protected:
    Environment       * m_environment;   ///< Gloperate environment to which the canvas belongs
    AbstractGLContext * m_openGLContext; ///< OpenGL context used for rendering onto the canvas
};


} // namespace gloperate
