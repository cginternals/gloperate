
#pragma once


#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/base/ExtendedProperties.h>


namespace globjects
{
    class Framebuffer;
}


namespace gloperate
{


class Stage;
class AbstractRenderTarget;
class ColorRenderTarget;
class DepthRenderTarget;
class DepthStencilRenderTarget;
class StencilRenderTarget;


/**
*  @brief
*    Interface that can be used for rendering stages and pipelines
*
*    A render stage is a stage that renders into render targets, given
*    a common viewport. This class can be used to instanciate the necessary inputs
*    for rendering stages. It can just be instanciated on a stage or pipeline
*    and it will add the inputs and outputs directly to the stage (the interface
*    itself is not an object in the hierarchy).
*
*    Render target inputs and outputs must be added dynamically to the stage
*    (see Stage::createInput) and will be picked up by the RenderInterface
*    automatically. They should reflect the targets the stage wants to render
*    into. To render into the specified targets, the stage should call obtainFBO()
*    and bind the returned framebuffer object before rendering.
*    
*    The viewport is initialized with an invalid width and height (i.e., -1.0
*    per component) which results in no rendering for rasterization stages and
*    full clearing for clear stages.
*/
class GLOPERATE_API RenderInterface
{
public:
    // Inputs
    Input<glm::vec4> viewport; ///< Viewport (in framebuffer coordinates)

    // Rendertarget Inputs -> subject to pipeline/stage designer
    // Rendertarget Outputs -> subject to pipeline/stage designer


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] stage
    *    Stage (must NOT be null)
    */
    RenderInterface(Stage * stage);

    /**
    *  @brief
    *    Destructor
    */
    ~RenderInterface();

    /**
    *  @brief
    *    Set value of all output render targets to their input render target counterparts
    */
    void updateRenderTargetOutputs();

    /**
    *  @brief
    *    Test if all registered render targets can be attached to a single FBO
    *
    *  @return
    *    'true', if all registered render targets are compatible using one single FBO, else 'false'
    */
    bool renderTargetsAreCompatible() const;

    /**
    *  @brief
    *    Get a configured framebuffer containing all render targets as attachments
    *
    *    Further, the draw buffers are updated on the framebuffer.
    *
    *  @param[in] fbo
    *    The user-defined framebuffer used for user-defined attachments
    *  @param[in] defaultFBO
    *    The default framebuffer used for default framebuffer attachments
    *
    *  @return
    *    The matching framebuffer; either fbo or defaultFBO, depending on the type of render target attachments
    *
    *  @remarks
    *    allRenderTargetsCompatible() is expected to return 'true'.
    */
    globjects::Framebuffer * obtainFBO() const;

    /**
    *  @brief
    *    Get a configured framebuffer containing one render target as attachment
    *
    *  @param[in] index
    *    The next color attachment index
    *  @param[in] renderTarget
    *    The render target to attach
    *
    *  @return
    *    The matching framebuffer; either a user-defined FBO or an default FBO, depending on the type of the render target attachment
    */
    globjects::Framebuffer * obtainFBO(size_t index, AbstractRenderTarget * renderTarget) const;

    /**
    *  @brief
    *    Get a configured framebuffer containing one render target as attachment
    *
    *  @param[in] index
    *    The next color attachment index
    *  @param[in] renderTarget
    *    The render target to attach
    *  @param[in] fbo
    *    The user-defined framebuffer used for user-defined attachments
    *  @param[in] defaultFBO
    *    Default default FBO for the current configuration
    *
    *  @return
    *    The matching framebuffer; either fbo or defaultFBO, depending on the type of the render target attachment
    */
    static globjects::Framebuffer * obtainFBO(size_t index, AbstractRenderTarget * renderTarget, globjects::Framebuffer * fbo, globjects::Framebuffer * defaultFBO);

    /**
    *  @brief
    *    Get all registered color render target inputs
    *
    *  @return
    *    The vector of color render target inputs
    */
    const std::vector<Input<ColorRenderTarget *> *> & colorRenderTargetInputs() const;

    /**
    *  @brief
    *    Get all registered depth render target inputs
    *
    *  @return
    *    The vector of depth render target inputs
    */
    const std::vector<Input<DepthRenderTarget *> *> & depthRenderTargetInputs() const;

    /**
    *  @brief
    *    Get all registered depth-stencil render target inputs
    *
    *  @return
    *    The vector of depth-stencil render target inputs
    */
    const std::vector<Input<DepthStencilRenderTarget *> *> & depthStencilRenderTargetInputs() const;

    /**
    *  @brief
    *    Get all registered stencil render target inputs
    *
    *  @return
    *    The vector of stencil render target inputs
    */
    const std::vector<Input<StencilRenderTarget *> *> & stencilRenderTargetInputs() const;

    /**
    *  @brief
    *    Get all registered color render target outputs
    *
    *  @return
    *    The vector of color render target outputs
    */
    const std::vector<Output<ColorRenderTarget *> *> & colorRenderTargetOutputs() const;

    /**
    *  @brief
    *    Get all registered depth render target outputs
    *
    *  @return
    *    The vector of depth render target outputs
    */
    const std::vector<Output<DepthRenderTarget *> *> & depthRenderTargetOutputs() const;

    /**
    *  @brief
    *    Get all registered depth-stencil render target outputs
    *
    *  @return
    *    The vector of depth-stencil render target outputs
    */
    const std::vector<Output<DepthStencilRenderTarget *> *> & depthStencilRenderTargetOutputs() const;

    /**
    *  @brief
    *    Get all registered stencil render target outputs
    *
    *  @return
    *    The vector of stencil render target outputs
    */
    const std::vector<Output<StencilRenderTarget *> *> & stencilRenderTargetOutputs() const;

    /**
    *  @brief
    *    Call function on each pair of color render target inputs and outputs
    *
    *  @param[in] callback
    *    Callback function
    *  @param[in] includeIncompletePairs
    *    If 'true', also incomplete pairs are passed to the callback (i.e., either input or output is 'null')
    */
    void pairwiseRenderTargetsDo(std::function<void(Input<ColorRenderTarget *> *, Output<ColorRenderTarget *> *)> callback, bool includeIncompletePairs = false);

    /**
    *  @brief
    *    Call function on each pair of depth render target inputs and outputs
    *
    *  @param[in] callback
    *    Callback function
    *  @param[in] includeIncompletePairs
    *    If 'true', also incomplete pairs are passed to the callback (i.e., either input or output is 'null')
    */
    void pairwiseRenderTargetsDo(std::function<void(Input<DepthRenderTarget *> *, Output<DepthRenderTarget *> *)> callback, bool includeIncompletePairs = false);

    /**
    *  @brief
    *    Call function on each pair of depth-stencil render target inputs and outputs
    *
    *  @param[in] callback
    *    Callback function
    *  @param[in] includeIncompletePairs
    *    If 'true', also incomplete pairs are passed to the callback (i.e., either input or output is 'null')
    */
    void pairwiseRenderTargetsDo(std::function<void(Input<DepthStencilRenderTarget *> *, Output<DepthStencilRenderTarget *> *)> callback, bool includeIncompletePairs = false);

    /**
    *  @brief
    *    Call function on each pair of stencil render target inputs and outputs
    *
    *  @param[in] callback
    *    Callback function
    *  @param[in] includeIncompletePairs
    *    If 'true', also incomplete pairs are passed to the callback (i.e., either input or output is 'null')
    */
    void pairwiseRenderTargetsDo(std::function<void(Input<StencilRenderTarget *> *, Output<StencilRenderTarget *> *)> callback, bool includeIncompletePairs = false);


public:
    /**
    *  @brief
    *    Initialize in OpenGL context
    *
    *  @see Stage::onContextInit
    */
    void onContextInit();

    /**
    *  @brief
    *    De-Initialize in OpenGL context
    *
    *  @see Stage::onContextDeinit()
    */
    void onContextDeinit();


protected:
    /**
    *  @brief
    *    Register render target input
    *
    *  @param[in] input
    *    Render target input (must NOT be null!)
    */
    void registerRenderTargetInput(AbstractSlot * input);

    /**
    *  @brief
    *    Register render target output
    *
    *  @param[in] output
    *    Render target output (must NOT be null!)
    */
    void registerRenderTargetOutput(AbstractSlot * output);


protected:
    std::unique_ptr<globjects::Framebuffer>           m_defaultFBO;                      ///< Default FBO for configuration
    std::unique_ptr<globjects::Framebuffer>           m_fbo;                             ///< Intermediate FBO for configuration
    std::vector<Input <ColorRenderTarget        *> *> m_colorRenderTargetInputs;         ///< List of input color render targets
    std::vector<Input <DepthRenderTarget        *> *> m_depthRenderTargetInputs;         ///< List of input depth render targets
    std::vector<Input <DepthStencilRenderTarget *> *> m_depthStencilRenderTargetInputs;  ///< List of input depth-stencil render targets
    std::vector<Input <StencilRenderTarget      *> *> m_stencilRenderTargetInputs;       ///< List of input stencil render targets
    std::vector<Output<ColorRenderTarget        *> *> m_colorRenderTargetOutputs;        ///< List of output color render targets (pass-through)
    std::vector<Output<DepthRenderTarget        *> *> m_depthRenderTargetOutputs;        ///< List of output depth render targets (pass-through)
    std::vector<Output<DepthStencilRenderTarget *> *> m_depthStencilRenderTargetOutputs; ///< List of output depth-stencil render targets (pass-through)
    std::vector<Output<StencilRenderTarget      *> *> m_stencilRenderTargetOutputs;      ///< List of output stencil render targets (pass-through)
};


} // namespace gloperate
