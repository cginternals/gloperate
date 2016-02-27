
#pragma once


#include <glm/glm.hpp>

#include <signalzeug/Signal.h>

#include <gloperate/gloperate_api.h>


namespace gloperate
{


class ViewerContext;
class AbstractGLContext;
class GLContextFormat;


/**
*  @brief
*    Base class for stages
*
*    A stage describes a functional entity that produces a certain set of outputs
*    and depends on a certain set of inputs. It can either be part of a pipeline
*    or stand on its own.
*/
class GLOPERATE_API Stage
{
public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the stage belongs (must NOT be null!)
    */
    Stage(ViewerContext * viewerContext);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~Stage();

    /**
    *  @brief
    *    Get viewer context
    *
    *  @return
    *    Viewer context to which the stage belongs (must NOT be null!)
    */
    ViewerContext * viewerContext() const;

    /**
    *  @brief
    *    Get required OpenGL context format
    *
    *  @return
    *    OpenGL context format expected by the stage
    */
    const GLContextFormat & requiredFormat() const;

    /**
    *  @brief
    *    Initialize in OpenGL context
    *
    *  @param[in] context
    *    OpenGL context used for rendering (must NOT null!)
    *
    *  @see onContextInit
    */
    void initContext(AbstractGLContext * context);

    /**
    *  @brief
    *    De-Initialize in OpenGL context
    *
    *  @param[in] context
    *    OpenGL context used for rendering (must NOT null!)
    *
    *  @see onContextDeinit
    */
    void deinitContext(AbstractGLContext * context);

    /**
    *  @brief
    *    Process stage
    *
    *  @param[in] context
    *    OpenGL context that is current (must NOT null!)
    *
    *  @see onProcess
    */
    void process(AbstractGLContext * context);

    // This interface is only a placeholder and will be replaced
    // by input/data-slots

    // Signals
    signalzeug::Signal<> outputInvalidated;
    void invalidateOutput();

    // Input data
    void setDeviceViewport(int x, int y, int w, int h);
    void setVirtualViewport(int x, int y, int w, int h);
    void setFrameCounter(int frame);
    void setTimeDelta(float delta);


protected:
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
    *
    *  @param[in] context
    *    OpenGL context used for rendering (must NOT null!)
    */
    virtual void onContextInit(AbstractGLContext * context);

    /**
    *  @brief
    *    De-Initialize in OpenGL context
    *
    *    This function is called when the OpenGL context is destroyed.
    *    The object must release its OpenGL objects at this point.
    *
    *  @param[in] context
    *    OpenGL context used for rendering (must NOT null!)
    *
    *  @see onContextInit()
    */
    virtual void onContextDeinit(AbstractGLContext * context);

    /**
    *  @brief
    *    Process stage
    *
    *    This function is called when the stage is executed.
    *    At this point, the stage is expected to process its
    *    inputs and create its output data.
    *
    *  @param[in] context
    *    OpenGL context that is current (must NOT null!)
    *
    *  @remarks
    *    The provided OpenGL context is already made current by
    *    the caller of this function, i.e., the viewer or parent
    *    pipeline. Therefore, the stage does not need to use
    *    the context pointer in most cases. However, if the stage
    *    manages its own context, it needs to rebind the former
    *    context at the end of its execution.
    */
    virtual void onProcess(AbstractGLContext * context);


protected:
    ViewerContext * m_viewerContext;    ///< Viewer context to which the stage belongs
    glm::ivec4      m_deviceViewport;   ///< Viewport (in real device coordinates)
    glm::ivec4      m_virtualViewport;  ///< Viewport (in virtual coordinates)
    unsigned long   m_frame;            ///< Frame counter
    float           m_timeDelta;        ///< Time since last update (in seconds)
};


} // namespace gloperate
