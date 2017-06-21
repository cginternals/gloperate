
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
class RenderTarget;


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
    Input<glm::vec4>                 viewport;    ///< Viewport (in framebuffer coordinates)

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

    size_t renderTargetInputSize() const;

    const std::vector<Input<RenderTarget *> *> & renderTargetInputs() const;

    Input<RenderTarget *> * renderTargetInput(size_t index) const;

    RenderTarget * inputRenderTarget(size_t index) const;

    size_t renderTargetOutputSize() const;

    const std::vector<Output<RenderTarget *> *> & renderTargetOutputs() const;

    Output<RenderTarget *> * renderTargetOutput(size_t index) const;

    RenderTarget * outputRenderTarget(size_t index) const;

    /**
    *  @brief
    *    Registers new render target input
    *
    *  @param[in] input
    *    New render target input
    */
    void addRenderTargetInput(Input<RenderTarget *> * input);

    /**
    *  @brief
    *    Registers new render target input
    *
    *  @param[in] input
    *    New render target input
    */
    void addRenderTargetOutput(Output<RenderTarget *> * input);

    void pairwiseRenderTargetsDo(std::function<void(Input <RenderTarget *> *, Output <RenderTarget *> *)> callback, bool includeIncompletePairs = false);

    globjects::Framebuffer * configureFBO(globjects::Framebuffer * fbo, globjects::Framebuffer * defaultFBO) const;

    static globjects::Framebuffer * configureFBO(size_t index, RenderTarget * renderTarget, globjects::Framebuffer * fbo, globjects::Framebuffer * defaultFBO);

protected:
    std::vector<Input <RenderTarget *> *> m_renderTargetInputs;  ///< List of input render targets
    std::vector<Output<RenderTarget *> *> m_renderTargetOutputs; ///< List of output render targets (pass-through)
};


} // namespace gloperate
