
#pragma once


#include <gloperate/base/GlmProperties.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace globjects {
    class Framebuffer;
}


namespace gloperate
{


/**
*  @brief
*    Pipeline container
*
*    The class is used as an interface between the canvas and the
*    current rendering stage or pipeline. It is itself a pipeline
*    and as such at the root of the parent chain. It defines the
*    inputs and outputs that connect the canvas to the rendering
*    stage or pipeline it contains.
*/
class GLOPERATE_API PipelineContainer : public Pipeline
{
public:
    // Render stage inputs
    Input<glm::vec4>                deviceViewport;  ///< Viewport (in real device coordinates)
    Input<glm::vec4>                virtualViewport; ///< Viewport (in virtual coordinates)
    Input<glm::vec3>                backgroundColor; ///< Background color (RGB)
    Input<int>                      frameCounter;    ///< Frame counter (number of frames)
    Input<float>                    timeDelta;       ///< Time delta since last frame (in seconds)
    Input<globjects::Framebuffer *> targetFBO;       ///< Target FBO. If null, the stage is supposed to render into the default frame buffer.

    // Render stage outputs
    Output<bool>                    rendered;        ///< 'true' if output has been rendered


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    */
    PipelineContainer(Environment * environment);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~PipelineContainer();

    /**
    *  @brief
    *    Get render stage
    *
    *  @return
    *    Render stage that renders into the current context (can be null)
    */
    Stage * renderStage() const;

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
    void setRenderStage(Stage * stage);


protected:
    void connect(Stage * stage, const std::string & name, AbstractSlot * source);
    void connect(AbstractSlot * input, Stage * stage, const std::string & name);
    void disconnect(Stage * stage, const std::string & name);


protected:
    Stage * m_renderStage; ///< Render stage that renders into the current context (can be null)
};


} // namespace gloperate
