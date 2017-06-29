
#include <gloperate-glkernel/stages/MultiFrameAggregationStage.h>

#include <glbinding/gl/functions.h>
#include <glbinding/gl/enum.h>

#include <globjects/base/StringTemplate.h>
#include <globjects/base/StaticStringSource.h>
#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Framebuffer.h>
#include <globjects/globjects.h>


namespace gloperate_glkernel
{


CPPEXPOSE_COMPONENT(MultiFrameAggregationStage, gloperate::Stage)


MultiFrameAggregationStage::MultiFrameAggregationStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, aggregationFBO("aggregationFBO", this)
, texture("texture", this)
, textureRerendered("textureRerendered", this)
, viewport("viewport", this)
, aggregationFactor("aggregationFactor", this)
, aggregatedFBO("aggregatedFBO", this)
{
}

MultiFrameAggregationStage::~MultiFrameAggregationStage()
{
}

void MultiFrameAggregationStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    m_quad = cppassist::make_unique<gloperate::ScreenAlignedQuad>();
}

void MultiFrameAggregationStage::onContextDeinit(gloperate::AbstractGLContext * /*context*/)
{
    m_quad.reset();
}

void MultiFrameAggregationStage::onProcess()
{
    if (!(*texture))
        return;

    gl::glViewport(
        0,              // Origin (0,0) because content was already shifted in main render pass
        0,              // Applying the origin again would shift the result again
        (*viewport).z,
        (*viewport).w
    );

    globjects::Framebuffer * fbo = *aggregationFBO;
    fbo->bind(gl::GL_FRAMEBUFFER);

    m_quad->setTexture(*texture);

    gl::glBlendColor(0.0f, 0.0f, 0.0f, *aggregationFactor);
    gl::glBlendFunc(gl::GL_CONSTANT_ALPHA, gl::GL_ONE_MINUS_CONSTANT_ALPHA);
    gl::glBlendEquation(gl::GL_FUNC_ADD);
    gl::glEnable(gl::GL_BLEND);
    gl::glDisable(gl::GL_DEPTH_TEST);

    m_quad->draw();

    gl::glDisable(gl::GL_BLEND);
    gl::glBlendFunc(gl::GL_SRC_ALPHA, gl::GL_ONE_MINUS_SRC_ALPHA);
    gl::glEnable(gl::GL_DEPTH_TEST);

    aggregatedFBO.setValue(*aggregationFBO);
}


} // namespace gloperate_glkernel
