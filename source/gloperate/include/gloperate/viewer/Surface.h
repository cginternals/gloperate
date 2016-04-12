
#pragma once


#include <glm/glm.hpp>

#include <reflectionzeug/Object.h>
#include <signalzeug/Signal.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class ViewerContext;
class AbstractGLContext;


/**
*  @brief
*    Representation of a surface into which can be rendered
*
*    A surface is attached to a window or offscreen context and handles the
*    actual rendering. It should be embedded by the windowing backend and
*    receives state changes from the outside (such as window size, mouse, or
*    keyboard events) and passes them on to the rendering components.
*/
class GLOPERATE_API Surface : public reflectionzeug::Object
{
public:
    signalzeug::Signal<> redrawNeeded;


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the surface belongs (must NOT be null!)
    */
    Surface(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Surface();

    /**
    *  @brief
    *    Get viewer context
    *
    *  @return
    *    Viewer context to which the surface belongs (must NOT be null!)
    */
    ViewerContext * viewerContext() const;

    /**
    *  @brief
    *    Get OpenGL context
    *
    *  @return
    *    OpenGL context used for rendering on the surface (can be null)
    *
    *  @remarks
    *    The returned context can be null if the surface has not been
    *    initialized yet, or the method is called between onContextDeinit()
    *    and onContextInit() when the context has been changed.
    *    Aside from that, there should always be a valid OpenGL context
    *    attached to the surface.
    */
    AbstractGLContext * openGLContext() const;

    /**
    *  @brief
    *    Set OpenGL context
    *
    *  @param[in] context
    *    OpenGL context used for rendering on the surface (can be null)
    *
    *  @remarks
    *    This function should only be called by the windowing backend.
    *    If the surface still has a valid context, onContextDeinit()
    *    will be called and the context pointer will be set to nullptr.
    *    Then, if the new context is valid, the context pointer will be
    *    set to that new context and onContextInit() will be invoked.
    */
    void setOpenGLContext(AbstractGLContext * context);

    /**
    *  @brief
    *    Get device viewport
    *
    *  @return
    *    Device viewport (actual device pixels)
    */
    virtual glm::ivec4 deviceViewport();

    /**
    *  @brief
    *    Get virtual viewport
    *
    *  @return
    *    Virtual viewport (virtual pixels)
    */
    virtual glm::ivec4 virtualViewport();

    /**
    *  @brief
    *    Update call once per main loop iteration
    */
    virtual void onUpdate();

    /**
    *  @brief
    *    Initialize in OpenGL context
    *
    *    This function is called whenever the OpenGL context has changed.
    *    This occurs not only when the viewer is created, but can also happen
    *    for example when switching between window and fullscreen mode.
    *    Therefore, the object must be aware that onContextInit() and
    *    onContextDeinit() can be called multiple times over its lifetime and
    *    must be able to create/restore all necessary OpenGL objects, such as
    *    textures and geometries, in this function.
    */
    virtual void onContextInit();

    /**
    *  @brief
    *    De-Initialize in OpenGL context
    *
    *    This function is called when the OpenGL context is destroyed.
    *    The object must release its OpenGL objects at this point.
    *
    *  @see onContextInit()
    */
    virtual void onContextDeinit();

    /**
    *  @brief
    *    Viewport changed
    *
    *    This function is called when the viewport has been changed,
    *    e.g., due to a resize of the window.
    *
    *  @param[in] deviceViewport
    *    Viewport (actual device pixels)
    *  @param[in] virtualViewport
    *    Viewport (virtual pixels)
    *
    *  @remarks
    *    On low-res devices, device size and virtual size will be equal.
    *    On high-res devices (e.g., retina displays), the device size will
    *    contain the actual framebuffer size in pixels, while the virtual
    *    size will be a smaller (virtual) pixel size. This can be facilitated
    *    for example by rendering complex scenes in the lower size and using
    *    the actual framebuffer size only for blitting onto the output device
    *    to improve rendering performance on high-resolution devices.
    */
    virtual void onViewport(
        const glm::ivec4 & deviceViewport
      , const glm::ivec4 & virtualViewport);

    /**
    *  @brief
    *    Background color changed
    *
    *    This function is called when the viewer has changed its background,
    *    e.g., because a new theme is being applied.
    *
    *  @param[in] red
    *    Red color component (0..1)
    *  @param[in] green
    *    Green color component (0..1)
    *  @param[in] blue
    *    Blue color component (0..1)
    */
    virtual void onBackgroundColor(float red, float green, float blue);

    /**
    *  @brief
    *    Render scene
    *
    *    This function is called when the viewer needs to redraw its content.
    *    Use it to render the actual scene you want to display.
    */
    virtual void onRender();

    /**
    *  @brief
    *    Key pressed
    *
    *    This function is called when a key has been pressed inside the viewer.
    *
    *  @param[in] key
    *    Virtual Key Code (see gloperate::Key)
    */
    virtual void onKeyPress(int key, int modifier);

    /**
    *  @brief
    *    Key released
    *
    *    This function is called when a key has been released inside the viewer.
    *
    *  @param[in] key
    *    Virtual Key Code (see gloperate::Key)
    */
    virtual void onKeyRelease(int key, int modifier);

    /**
    *  @brief
    *    Mouse moved
    *
    *    This function is called when the mouse has been moved inside the viewer.
    *
    *  @param[in] pos
    *    Mouse position (real device coordinates)
    */
    virtual void onMouseMove(const glm::ivec2 & pos);

    /**
    *  @brief
    *    Mouse button pressed
    *
    *    This function is called when a mouse button has been pressed inside the viewer.
    *
    *  @param[in] button
    *    Mouse button (see gloperate::MouseButton)
    *  @param[in] pos
    *    Mouse position (real device coordinates)
    */
    virtual void onMousePress(int button, const glm::ivec2 & pos);

    /**
    *  @brief
    *    Mouse button released
    *
    *    This function is called when a mouse button has been released inside the viewer.
    *
    *  @param[in] button
    *    Mouse button (see gloperate::MouseButton)
    *  @param[in] pos
    *    Mouse position (real device coordinates)
    */
    virtual void onMouseRelease(int button, const glm::ivec2 & pos);

    /**
    *  @brief
    *    Mouse wheel event
    *
    *    This function is called when the mouse wheel has been scrolled.
    *
    *  @param[in] delta
    *    Wheel delta
    *  @param[in] pos
    *    Mouse position (real device coordinates)
    */
    virtual void onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos);


protected:
    ViewerContext     * m_viewerContext; ///< Viewer context to which the surface belongs
    AbstractGLContext * m_openGLContext; ///< OpenGL context used for rendering on the surface
};


} // namespace gloperate
