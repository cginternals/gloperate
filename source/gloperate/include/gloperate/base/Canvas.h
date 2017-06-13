
#pragma once


#include <string>
#include <mutex>

#include <glm/vec4.hpp>
#include <glm/fwd.hpp>

#include <cppexpose/reflection/Object.h>
#include <cppexpose/variant/Variant.h>
#include <cppexpose/signal/Signal.h>
#include <cppexpose/signal/ScopedConnection.h>

#include <gloperate/base/ChronoTimer.h>


namespace globjects
{
    class Framebuffer;
}


namespace gloperate
{


class Environment;
class AbstractGLContext;
class Stage;
class AbstractSlot;
class MouseDevice;
class KeyboardDevice;


/**
*  @brief
*    Representation of a canvas onto which can be rendered
*
*    A canvas is attached to a window or offscreen context and handles the
*    actual rendering. It should be embedded by the windowing backend and
*    receives state changes from the outside (such as window size, mouse,
*    or keyboard events) and passes them on to the rendering components.
*/
class GLOPERATE_API Canvas : public cppexpose::Object
{
public:
    // Must be emitted only from the UI thread
    cppexpose::Signal<> redraw; ///< Called when the canvas needs to be redrawn


public:
    //@{
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the canvas belongs (must NOT be null!)
    */
    Canvas(Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    ~Canvas();
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
    *    Get render stage
    *
    *  @return
    *    Render stage that renders into the current context (can be null)
    */
    const Stage * renderStage() const;
    Stage * renderStage();
    //@}

    //@{
    /**
    *  @brief
    *    Set render stage
    *
    *  @param[in] stage
    *    Render stage that renders into the current context (can be null)
    */
    void setRenderStage(std::unique_ptr<Stage> && stage);

    /**
    *  @brief
    *    Load render stage
    *
    *  @param[in] name
    *    Name of render stage
    */
    void loadRenderStage(const std::string & name);
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
    //@}

    //@{
    /**
    *  @brief
    *    Update virtual time (must be called from UI thread)
    *
    *  @remarks
    *    This function determines the time delta since the last call to
    *    the function and updates the internal virtual time. This is
    *    passed on to the render stage to allow for continuous updates
    *    of the virtual scene. If a pipeline depends on the virtual time
    *    or time delta inputs and in turn invalidates its render outputs,
    *    a redraw will be scheduled. Otherwise, only the virtual time is
    *    updated regularly, but no redraw occurs.
    */
    void updateTime();

    /**
    *  @brief
    *    Set viewport (must be called from UI thread)
    *
    *  @param[in] deviceViewport
    *    Viewport (in real device coordinates)
    *  @param[in] virtualViewport
    *    Viewport (in virtual coordinates)
    */
    void setViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport);

    /**
    *  @brief
    *    Get device viewport
    *
    *  @return
    *    The device viewport
    */
    const glm::vec4 & deviceViewport() const;

    /**
    *  @brief
    *    Get virtual viewport
    *
    *  @return
    *    The virtual viewport
    */
    const glm::vec4 & virtualViewport() const;

    /**
    *  @brief
    *    Perform rendering (must be called from render thread)
    *
    *  @param[in] targetFBO
    *    Framebuffer into which is rendered
    */
    void render(globjects::Framebuffer * targetFBO);

    /**
    *  @brief
    *    Promote keyboard press event (must be called from UI thread)
    *
    *  @param[in] key
    *    Key (gloperate key code)
    *  @param[in] modifier
    *    Modifiers (gloperate modifier codes)
    */
    void promoteKeyPress(int key, int modifier);

    /**
    *  @brief
    *    Promote keyboard release event (must be called from UI thread)
    *
    *  @param[in] key
    *    Key (gloperate key code)
    *  @param[in] modifier
    *    Modifiers (gloperate modifier codes)
    */
    void promoteKeyRelease(int key, int modifier);

    /**
    *  @brief
    *    Promote mouse move event (must be called from UI thread)
    *
    *  @param[in] pos
    *    Mouse position
    */
    void promoteMouseMove(const glm::ivec2 & pos);

    /**
    *  @brief
    *    Promote mouse press event (must be called from UI thread)
    *
    *  @param[in] button
    *    Mouse button (gloperate button code)
    *  @param[in] pos
    *    Mouse position
    */
    void promoteMousePress(int button, const glm::ivec2 & pos);

    /**
    *  @brief
    *    Promote mouse release event (must be called from UI thread)
    *
    *  @param[in] button
    *    Mouse button (gloperate button code)
    *  @param[in] pos
    *    Mouse position
    */
    void promoteMouseRelease(int button, const glm::ivec2 & pos);

    /**
    *  @brief
    *    Promote mouse wheel event (must be called from UI thread)
    *
    *  @param[in] delta
    *    Wheel delta
    *  @param[in] pos
    *    Mouse position
    */
    void promoteMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos);
    //@}


protected:
    /**
    *  @brief
    *    Check if a redraw is required
    *
    *  @remarks
    *    This function checks if the render stage needs to be redrawn
    *    and invokes the redraw signal if that is the case.
    */
    void checkRedraw();

    /**
    *  @brief
    *    Promote changes of input slots
    */
    void promoteChangedInputs();

    /**
    *  @brief
    *    Called when an input on the current stage has changed
    *
    *  @param[in] slot
    *    Input slot
    */
    void stageInputChanged(AbstractSlot * slot);

    // Scripting functions
    void scr_onStageInputChanged(const cppexpose::Variant & func);
    cppexpose::Variant scr_getSlotTypes(const std::string & path);
    std::string scr_createStage(const std::string & path, const std::string & name, const std::string & type);
    void scr_removeStage(const std::string & path, const std::string & name);
    void scr_createSlot(const std::string & path, const std::string & slot, const std::string & slotType, const std::string & type);
    cppexpose::Variant scr_getConnections(const std::string & path);
    void scr_createConnection(const std::string & sourcePath, const std::string & sourceSlot, const std::string & destPath, const std::string & destSlot);
    void scr_removeConnection(const std::string & path, const std::string & slot);
    cppexpose::Variant scr_getStage(const std::string & path);
    cppexpose::Variant scr_getSlot(const std::string & path, const std::string & slot);
    cppexpose::Variant scr_getValue(const std::string & path, const std::string & slot);
    void scr_setValue(const std::string & path, const std::string & slot, const cppexpose::Variant & value);

    // Helper functions
    Stage * getStageObject(const std::string & path) const;
    cppexpose::Variant getSlotStatus(const std::string & path, const std::string & slot);


protected:
    Environment                   * m_environment;            ///< Gloperate environment to which the canvas belongs
    AbstractGLContext             * m_openGLContext;          ///< OpenGL context used for rendering onto the canvas
    bool                            m_initialized;            ///< 'true' if the context has been initialized and the viewport has been set, else 'false'
    gloperate::ChronoTimer          m_clock;                  ///< Time measurement
    glm::vec4                       m_deviceViewport;         ///< Viewport (in real device coordinates)
    glm::vec4                       m_virtualViewport;        ///< Viewport (in virtual coordinates)
    float                           m_timeDelta;              ///< Time delta since the last update (in seconds)
    std::unique_ptr<Stage>          m_renderStage;            ///< Render stage that renders into the canvas
    std::unique_ptr<Stage>          m_oldStage;               ///< Old render stage, will be destroyed on the next render call
    std::unique_ptr<MouseDevice>    m_mouseDevice;            ///< Device for Mouse Events
    std::unique_ptr<KeyboardDevice> m_keyboardDevice;         ///< Device for Keyboard Events
    bool                            m_replaceStage;           ///< 'true' if the stage has just been replaced, else 'false'
    std::mutex                      m_mutex;                  ///< Mutex for separating main and render thread
    cppexpose::ScopedConnection     m_inputChangedConnection; ///< Connection for the inputChanged-signal of the current stage
    cppexpose::Function             m_inputChangedCallback;   ///< Script function that is called on inputChanged (slot, status)
    std::vector<AbstractSlot *>     m_changedInputs;          ///< List of changed input slots
    std::mutex                      m_changedInputMutex;      ///< Mutex to access m_changedInputs
};


} // namespace gloperate
