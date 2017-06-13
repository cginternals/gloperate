
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
, sourceTexture("sourceTexture", this)
, targetTexture("targetTexture", this)
, viewport("viewport", this)
, aggregationFactor("aggregationFactor", this)
, aggregatedTexture("aggregatedTexture", this)
{
}

MultiFrameAggregationStage::~MultiFrameAggregationStage()
{
}

void MultiFrameAggregationStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    m_triangle = cppassist::make_unique<gloperate::ScreenAlignedTriangle>();
    m_fbo = cppassist::make_unique<globjects::Framebuffer>();
}

void MultiFrameAggregationStage::onContextDeinit(gloperate::AbstractGLContext * /*context*/)
{
    m_triangle = nullptr;
    m_fbo = nullptr;
}

void MultiFrameAggregationStage::onProcess()
{
    gl::glViewport(
        0,              // Origin (0,0) because content was already shifted in main render pass
        0,              // Applying the origin again would shift the result again
        (*viewport).z,
        (*viewport).w
    );

    m_fbo->bind(gl::GL_FRAMEBUFFER);

    gl::glBlendColor(0.0f, 0.0f, 0.0f, *aggregationFactor);
    gl::glBlendFunc(gl::GL_CONSTANT_ALPHA, gl::GL_ONE_MINUS_CONSTANT_ALPHA);
    gl::glBlendEquation(gl::GL_FUNC_ADD);
    gl::glEnable(gl::GL_BLEND);

    m_triangle->setTexture(*sourceTexture);
    m_triangle->draw();

    gl::glDisable(gl::GL_BLEND);

    aggregatedTexture.setValue(*targetTexture);
}


} // namespace gloperate_glkernel
