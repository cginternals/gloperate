
#pragma once


#include <gloperate/base/AbstractCanvas.h>
#include <gloperate/pipeline/PipelineContainer.h>
#include <gloperate/input/constants.h>


namespace gloperate
{


class Stage;
class MouseDevice;
class KeyboardDevice;


/**
*  @brief
*    Default canvas renderer for gloperate
*/
class GLOPERATE_API Canvas : public AbstractCanvas
{
public:
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
    virtual ~Canvas();

    /**
    *  @brief
    *    Get pipeline container
    *
    *  @return
    *    Pipeline container (never null)
    */
    const PipelineContainer * pipelineContainer() const;

    /**
    *  @brief
    *    Get pipeline container
    *
    *  @return
    *    Pipeline container (never null)
    */
    PipelineContainer * pipelineContainer();

    /**
    *  @brief
    *    Get render stage
    *
    *  @return
    *    Render stage that renders into the current context (can be null)
    */
    const Stage * renderStage() const;

    /**
    *  @brief
    *    Get render stage
    *
    *  @return
    *    Render stage that renders into the current context (can be null)
    */
    Stage * renderStage();

    /**
    *  @brief
    *    Release and return current render stage
    *
    *  @return
    *    The current render stage
    *
    *  @remarks
    *    The pipeline container is always empty after this function call.
    *    If the return value is not used, the stage gets deleted immediately
    *    (potentionally leaking OpenGL resources or crash upon deletion
    *    because of a missing current OpenGL context).
    */
    std::unique_ptr<Stage> obtainRenderStage();

    /**
    *  @brief
    *    Set render stage
    *
    *  @param[in] stage
    *    Render stage that renders into the current context (can be null)
    *
    *  @remarks
    *    When setting a new render stage, the old render stage is destroyed.
    *    The canvas takes ownership over the stage.
    */
    void setRenderStage(std::unique_ptr<Stage> && stage);

    // Virtual AbstractCanvas functions
    virtual void onRender(globjects::Framebuffer * targetFBO) override;
    virtual void onUpdate() override;
    virtual void onContextInit() override;
    virtual void onContextDeinit() override;
    virtual void onViewport(const glm::vec4 & deviceViewport, const glm::vec4 & virtualViewport) override;
    virtual void onSaveViewport() override;
    virtual void onResetViewport() override;
    virtual void onBackgroundColor(float red, float green, float blue) override;
    virtual void onKeyPress(int key, int modifier) override;
    virtual void onKeyRelease(int key, int modifier) override;
    virtual void onMouseMove(const glm::ivec2 & pos) override;
    virtual void onMousePress(int button, const glm::ivec2 & pos) override;
    virtual void onMouseRelease(int button, const glm::ivec2 & pos) override;
    virtual void onMouseWheel(const glm::vec2 & delta, const glm::ivec2 & pos) override;
    virtual const glm::vec4 & savedDeviceViewport() const override;


protected:
    PipelineContainer               m_pipelineContainer; ///< Container for the rendering stage or pipeline
    unsigned long                   m_frame;             ///< Frame counter
    std::unique_ptr<MouseDevice>    m_mouseDevice;       ///< Device for Mouse Events
    std::unique_ptr<KeyboardDevice> m_keyboardDevice;    ///< Device for Keyboard Events
    glm::vec4                       m_deviceViewport;    ///< Last known device viewport configuration
    glm::vec4                       m_virtualViewport;   ///< Last known virtual viewport configuration
    glm::vec4                       m_savedDeviceVP;     ///< Saved device viewport configuration
    glm::vec4                       m_savedVirtualVP;    ///< Saved virtual viewport configuration
};


} // namespace gloperate
