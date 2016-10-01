
#pragma once


#include <glm/glm.hpp>

#include <cppexpose/reflection/Object.h>
#include <cppexpose/signal/Signal.h>
#include <cppexpose/reflection/Object.h>
#include <cppexpose/variant/Variant.h>

#include <gloperate/gloperate_api.h>


namespace globjects {
    class Framebuffer;
}


namespace gloperate
{


class Environment;
class AbstractGLContext;
class ImageExporter;
class AbstractVideoExporter;


/**
*  @brief
*    Representation of a canvas onto which can be rendered
*
*    A canvas is attached to a window or offscreen context and handles the
*    actual rendering. It should be embedded by the windowing backend and
*    receives state changes from the outside (such as window size, mouse, or
*    keyboard events) and passes them on to the rendering components.
*/
class GLOPERATE_API AbstractCanvas : public cppexpose::Object
{
public:
    cppexpose::Signal<> redraw; ///< Called when the canvas needs to be redrawn
    cppexpose::Signal<> wakeup; ///< Called when the main loop need to wake up


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the canvas belongs (must NOT be null!)
    */
    AbstractCanvas(Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~AbstractCanvas();

    /**
    *  @brief
    *    Get gloperate environment
    *
    *  @return
    *    Environment to which the canvas belongs (must NOT be null!)
    */
    Environment * environment() const;

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
    AbstractGLContext * openGLContext() const;

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

    /**
    *  @brief
    *    Render this canvas to an image
    *
    *  @param[in] filename
    *    Name of output image file
    *  @param[in] width
    *    Width (in pixels) of output image
    *  @param[in] height
    *    Height (in pixels) of output image
    *  @param[in] renderIterations
    *    Number of render iterations
    */
    void exportImage(std::string filename, int width, int height, int renderIterations);

    /**
    *  @brief
    *    Render this canvas to a video
    *
    *  @param[in] filename
    *    Name of output video file
    *  @param[in] width
    *    Width (in pixels) of output video
    *  @param[in] height
    *    Height (in pixels) of output video
    *  @param[in] fps
    *    Frames per second of output video
    *  @param[in] seconds
    *    Length (in seconds) of output video
    *  @param[in] backend
    *    Name of video backend to use
    */
    void exportVideo(std::string filename, int width, int height, int fps, int seconds, std::string backend = "FFMPEGVideoExporter");

    /**
    *  @brief
    *    Get current video export progress in percent
    *
    *  @return
    *    Percent of the current video export progress
    *
    *  @remarks
    *    When nothing is currently being exported, this function returns 0.
    */
    int exportProgress();

    /**
    *  @brief
    *    Get the available plugin names for video export backends
    *
    *  @return
    *    Vector of the available plugin names for video export backends
    */
    cppexpose::VariantArray videoExporterPlugins();

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
        const glm::vec4 & deviceViewport
      , const glm::vec4 & virtualViewport);

    /**
    *  @brief
    *    Save current viewport
    *
    *    This function is called to save the current viewport configuration
    *    in order to restore it later. This is used for example when
    *    making screenshots with resolutions that are different to the
    *    current window's viewport. The implementation is supposed to
    *    save the last values it has been provided with by onViewport.
    */
    virtual void onSaveViewport();

    /**
    *  @brief
    *    Reset viewport to saved value
    *
    *    This function is called to reset the viewport to previously saved
    *    values (see onSaveViewport). The implementation is supposed to reset
    *    the viewport to the values present on the last call to onSaveViewport.
    */
    virtual void onResetViewport();

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
    *
    *  @param[in] targetFBO
    *    Target FBO (can be null)
    */
    virtual void onRender(globjects::Framebuffer * targetFBO = nullptr);

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
    Environment           * m_environment;   ///< Gloperate environment to which the canvas belongs
    AbstractGLContext     * m_openGLContext; ///< OpenGL context used for rendering onto the canvas
    ImageExporter         * m_imageExporter; ///< Tool for exporting canvas to image file
    AbstractVideoExporter * m_videoExporter; ///< Tool for rendering canvas to video file
    bool                    m_requestImage;  ///< Flag to request a ImageExporter call during next render step
    bool                    m_requestVideo;  ///< Flag to request a VideoTool call during next render step
};


} // namespace gloperate
