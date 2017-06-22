
#pragma once


#include <gloperate/gloperate_api.h>

#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/base/ExtendedProperties.h>


namespace globjects
{


class Framebuffer;


} // namespace globjects


namespace gloperate
{


class Stage;
class AbstractRenderTarget;
class ColorRenderTarget;
class DepthRenderTarget;
class StencilRenderTarget;


/**
*  @brief
*    Interface that can be used for rendering stages and pipelines
*
*    A render stage is a stage that renders into render targets, given
*    a common viewport.
*
*    This class can be used to instanciate the necessary inputs and outputs
*    for rendering stages. It can just be instanciated on a stage or pipeline
*    and it will add the inputs and outputs directly to the stage (the interface
*    itself is not an object in the hierarchy).
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

    bool allRenderTargetsCompatible() const;

    const std::vector<Input<ColorRenderTarget *> *> & colorRenderTargetInputs() const;
    const std::vector<Input<DepthRenderTarget *> *> & depthRenderTargetInputs() const;
    const std::vector<Input<StencilRenderTarget *> *> & stencilRenderTargetInputs() const;

    Input<ColorRenderTarget *> * colorRenderTargetInput(size_t index) const;
    Input<DepthRenderTarget *> * depthRenderTargetInput(size_t index) const;
    Input<StencilRenderTarget *> * stencilRenderTargetInput(size_t index) const;

    ColorRenderTarget * inputColorRenderTarget(size_t index) const;
    DepthRenderTarget * inputDepthRenderTarget(size_t index) const;
    StencilRenderTarget * inputStencilRenderTarget(size_t index) const;

    const std::vector<Output<ColorRenderTarget *> *> & colorRenderTargetOutputs() const;
    const std::vector<Output<DepthRenderTarget *> *> & depthRenderTargetOutputs() const;
    const std::vector<Output<StencilRenderTarget *> *> & stencilRenderTargetOutputs() const;

    Output<ColorRenderTarget *> * colorRenderTargetOutput(size_t index) const;
    Output<DepthRenderTarget *> * depthRenderTargetOutput(size_t index) const;
    Output<StencilRenderTarget *> * stencilRenderTargetOutput(size_t index) const;

    ColorRenderTarget * outputColorRenderTarget(size_t index) const;
    DepthRenderTarget * outputDepthRenderTarget(size_t index) const;
    StencilRenderTarget * outputStencilRenderTarget(size_t index) const;

    /**
    *  @brief
    *    Registers new render target input
    *
    *  @param[in] input
    *    New render target input
    */
    void addRenderTargetInput(Input<ColorRenderTarget *> * input);
    void addRenderTargetInput(Input<DepthRenderTarget *> * input);
    void addRenderTargetInput(Input<StencilRenderTarget *> * input);

    /**
    *  @brief
    *    Registers new render target input
    *
    *  @param[in] input
    *    New render target input
    */
    void addRenderTargetOutput(Output<ColorRenderTarget *> * input);
    void addRenderTargetOutput(Output<DepthRenderTarget *> * input);
    void addRenderTargetOutput(Output<StencilRenderTarget *> * input);

    void pairwiseRenderTargetsDo(std::function<void(Input<ColorRenderTarget *> *, Output<ColorRenderTarget *> *)> callback, bool includeIncompletePairs = false);
    void pairwiseRenderTargetsDo(std::function<void(Input<DepthRenderTarget *> *, Output<DepthRenderTarget *> *)> callback, bool includeIncompletePairs = false);
    void pairwiseRenderTargetsDo(std::function<void(Input<StencilRenderTarget *> *, Output<StencilRenderTarget *> *)> callback, bool includeIncompletePairs = false);

    globjects::Framebuffer * configureFBO(globjects::Framebuffer * fbo, globjects::Framebuffer * defaultFBO) const;

    static globjects::Framebuffer * configureFBO(size_t index, AbstractRenderTarget * renderTarget, globjects::Framebuffer * fbo, globjects::Framebuffer * defaultFBO);

protected:
    std::vector<Input <ColorRenderTarget   *> *> m_colorRenderTargetInputs;    ///< List of input color render targets
    std::vector<Input <DepthRenderTarget   *> *> m_depthRenderTargetInputs;    ///< List of input depth render targets
    std::vector<Input <StencilRenderTarget *> *> m_stencilRenderTargetInputs;  ///< List of input depth-stencil render targets
    std::vector<Output<ColorRenderTarget   *> *> m_colorRenderTargetOutputs;   ///< List of output color render targets (pass-through)
    std::vector<Output<DepthRenderTarget   *> *> m_depthRenderTargetOutputs;   ///< List of output depth render targets (pass-through)
    std::vector<Output<StencilRenderTarget *> *> m_stencilRenderTargetOutputs; ///< List of output depth-stencil render targets (pass-through)
};


} // namespace gloperate
