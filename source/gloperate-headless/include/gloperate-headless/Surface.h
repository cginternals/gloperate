
#pragma once

#include <memory>
#include <queue>
#include <set>
#include <string>

#include <glm/vec2.hpp>

#include <gloperate/base/GLContextFormat.h>

#include <gloperate-headless/gloperate-headless_api.h>


namespace egl
{


using EGLDisplay = void*;
using EGLSurface = void*;


} // namespace egl


namespace gloperate
{
    class AbstractGLContext;
}


namespace gloperate_headless
{


class GLContext;
class SurfaceEvent;
class KeyEvent;
class MouseEvent;
class ResizeEvent;
class PaintEvent;
class CloseEvent;


/**
*  @brief
*    Surface with OpenGL context
*/
class GLOPERATE_HEADLESS_API Surface
{
    friend class Application;


public:
    /**
    *  @brief
    *    Get list of surfaces
    *
    *  @return
    *    List of open surfaces
    */
    static const std::set<Surface*> & instances();


private:
    static std::set<Surface*> s_instances;   ///< List of surface instances


public:
    /**
    *  @brief
    *    Constructor
    */
    Surface(egl::EGLDisplay display);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Surface();

    /**
    *  @brief
    *    Set OpenGL context format
    *
    *  @param[in] format
    *    The desired OpenGL context format
    *
    *  @return
    *    'true' if the format could be set, else 'false'
    *
    *  @remarks
    *    The context format can only be set before the surface
    *    has been created. Afterwards, the function will fail
    *    and the context format will not be changed.
    */
    bool setContextFormat(const gloperate::GLContextFormat & format);

    /**
    *  @brief
    *    Create surface
    *
    *  @return
    *    'true' if surface could be created, else 'false'
    *
    *  @remarks
    *    If the surface has already been created, this function will fail.
    */
    bool create();

    /**
    *  @brief
    *    Destroy surface
    */
    void destroy();

    /**
    *  @brief
    *    Get OpenGL context
    *
    *  @return
    *    OpenGL context (can be nullptr)
    */
    const GLContext * context() const;

    /**
    *  @brief
    *    Get OpenGL context
    *
    *  @return
    *    OpenGL context (can be nullptr)
    */
    GLContext * context();

    /**
    *  @brief
    *    Dispose ressources of surface
    */
    void dispose();

    /**
    *  @brief
    *    Get surface size
    *
    *  @return
    *    Surface size (in pixels)
    */
    glm::ivec2 size() const;

    /**
    *  @brief
    *    Set surface size
    *
    *  @param[in] width
    *    Surface width (in pixels)
    *  @param[in] height
    *    Surface height (in pixels)
    */
    void setSize(int width, int height);

    /**
    *  @brief
    *    Get surface title
    *
    *  @return
    *    Surface title
    */
    const std::string & title() const;

    /**
    *  @brief
    *    Set surface title
    *
    *  @param[in] title
    *    Surface title
    */
    void setTitle(const std::string & title);

    /**
    *  @brief
    *    Check if application quits when surfaces gets closed
    *
    *  @return
    *    'true' if surface quits the application when closed, else 'false'
    */
    bool quitsOnDestroy() const;

    /**
    *  @brief
    *    Set if application quits when the surface gets closed
    *
    *  @param[in] quitOnDestroy
    *    'true' if surface quits the application when closed, else 'false'
    *
    *  @remarks
    *    If enabled, this causes an application wide quit message to be posted
    *    when the surface gets destroyed. Hence, the MainLoop will be quit
    *    and all other remaining surfaces destroyed.
    */
    void setQuitOnDestroy(bool quitOnDestroy);

    /**
    *  @brief
    *    Schedule a repaint on the surface
    *
    *  @remarks
    *    When calling this function, a redraw-event will be sent to the
    *    message queue, causing the surface to be repainted within the
    *    next event processing.
    */
    void repaint();

    /**
    *  @brief
    *    Swap front and back buffer
    */
    void swap();

    /**
    *  @brief
    *    Called once every mainloop iteration
    */
    void idle();


protected:
    /**
    *  @brief
    *    Get EGL surface handle
    *
    *  @return
    *    EGL surface, can be nullptr
    */
    egl::EGLSurface internalSurface() const;

    /**
    *  @brief
    *    Add event to the surface's event queue
    *
    *  @param[in] event
    *    Event (can be nullptr)
    */
    void queueEvent(std::unique_ptr<SurfaceEvent> && event);

    /**
    *  @brief
    *    Check if surface has events waiting
    *
    *  @return
    *    'true' if events are waiting in the surface's event queue, else 'false'
    */
    bool hasPendingEvents();

    /**
    *  @brief
    *    Process all waiting events and dispatch them to event handling
    */
    void processEvents();

    /**
    *  @brief
    *    Check for repaint event
    *
    *  @remarks
    *    If repaint() has been called on the surface, a repaint event
    *    will be added to the surface's event queue. This needs to be
    *    done as a separate call to avoid event processing to get stuck
    *    in an endless loop. Make sure to call Application::pollEvents()
    *    and updateRepaintEvent() in turn.
    */
    void updateRepaintEvent();

    /**
    *  @brief
    *    Handle surface event
    *
    *  @param[in] event
    *    Surface event
    */
    void handleEvent(SurfaceEvent & event);

    /**
    *  @brief
    *    Remove all waiting events from the queue
    */
    void clearEventQueue();

    /**
    *  @brief
    *    Recreates EGL surface using current settings
    *
    *  @return
    *    'true' on success, else 'false'
    */
    bool recreateSurface();

    /**
    *  @brief
    *    Create internal surface and OpenGL context
    *
    *  @param[in] format
    *    The desired OpenGL context format
    *  @param[in] width
    *    Surface width (in pixels)
    *  @param[in] height
    *    Surface height (in pixels)
    *  @param[in] display
    *    EGL display handle, can be nullptr
    *
    *  @return
    *    'true' if context could be created, else 'false'
    *
    *  @remarks
    *    This function will actually create a new surface with the given context
    *    format, so any previously obtained surface IDs will be rendered invalid.
    */
    bool createInternalSurface(const gloperate::GLContextFormat & format, int width, int height, bool setContextActive, egl::EGLDisplay display);

    /**
    *  @brief
    *    Destroy internal surface and OpenGL context
    *
    *  @remarks
    *    This function will actually destroy the current surface.
    */
    void destroyInternalSurface();

    // Event handlers, to be overwritten in derived classes
    virtual void onContextInit();
    virtual void onContextDeinit();
    virtual void onResize(ResizeEvent & event);
    virtual void onPaint(PaintEvent & event);
    virtual void onKeyPress(KeyEvent & event);
    virtual void onKeyRelease(KeyEvent & event);
    virtual void onMousePress(MouseEvent & event);
    virtual void onMouseMove(MouseEvent & event);
    virtual void onMouseRelease(MouseEvent & event);
    virtual void onScroll(MouseEvent & event);
    virtual void onClose(CloseEvent & event);
    virtual void onIdle();


protected:
    std::string                               m_title;         ///< Surface title
    gloperate::GLContextFormat                m_format;        ///< The desired OpenGL context format

    egl::EGLDisplay                           m_display;       ///< EGL display (can be nullptr)
    std::queue<std::unique_ptr<SurfaceEvent>> m_eventQueue;    ///< List of events to be processed by the surface
    glm::ivec2                                m_size;          ///< Size of surface
    bool                                      m_quitOnDestroy; ///< Quit application when surface is closed?
    bool                                      m_needsRepaint;  ///< Has a repaint be scheduled?
    std::unique_ptr<GLContext>                m_context;       ///< OpenGL context & EGL Surface (can be nullptr)
};


} // namespace gloperate_headless
