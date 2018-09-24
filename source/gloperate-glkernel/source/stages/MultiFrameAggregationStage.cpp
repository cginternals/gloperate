
#include <gloperate-glkernel/stages/MultiFrameAggregationStage.h>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/Framebuffer.h>
#include <globjects/FramebufferAttachment.h>
#include <globjects/AttachedTexture.h>

#include <gloperate/rendering/ColorRenderTarget.h>
#include <gloperate/rendering/AttachmentType.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(MultiFrameAggregationStage, gloperate::Stage)


MultiFrameAggregationStage::MultiFrameAggregationStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, renderInterface  (this)
, intermediateFrame("intermediateFrame", this)
, aggregationFactor("aggregationFactor", this)
{
}

MultiFrameAggregationStage::~MultiFrameAggregationStage()
{
}

void MultiFrameAggregationStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    m_triangle = cppassist::make_unique<gloperate::ScreenAlignedTriangle>();

    renderInterface.onContextInit();
}

void MultiFrameAggregationStage::onContextDeinit(gloperate::AbstractGLContext * /*context*/)
{
    m_triangle = nullptr;

    renderInterface.onContextDeinit();
}

void MultiFrameAggregationStage::onProcess()
{
    if (!renderInterface.allRenderTargetsCompatible())
    {
        cppassist::warning("gloperate") << "Framebuffer configuration not compatible";

        return;
    }

    auto fbo = renderInterface.obtainFBO();

    gl::glViewport(
        renderInterface.viewport->x,
        renderInterface.viewport->y,
        renderInterface.viewport->z,
        renderInterface.viewport->w
    );

    fbo->bind(gl::GL_FRAMEBUFFER);
    fbo->printStatus(true);

    gl::glBlendColor(0.0f, 0.0f, 0.0f, *aggregationFactor);
    gl::glBlendFunc(gl::GL_CONSTANT_ALPHA, gl::GL_ONE_MINUS_CONSTANT_ALPHA);
    gl::glBlendEquation(gl::GL_FUNC_ADD);
    gl::glEnable(gl::GL_BLEND);
    gl::glDisable(gl::GL_DEPTH_TEST);

    m_triangle->setTexture(*intermediateFrame);
    m_triangle->draw();

    gl::glDisable(gl::GL_BLEND);
    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);
    gl::glEnable(gl::GL_DEPTH_TEST);

    renderInterface.updateRenderTargetOutputs();
}


} // namespace gloperate_glkernel
