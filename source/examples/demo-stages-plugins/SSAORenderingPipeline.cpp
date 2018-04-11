
#include "SSAORenderingPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/TextureRenderTargetStage.h>
#include <gloperate/stages/base/TextureFromRenderTargetExtractionStage.h>

#include <gloperate-glkernel/stages/HemisphereDistributionKernelStage.h>
#include <gloperate-glkernel/stages/NoiseKernelStage.h>

#include <glbinding/gl/enum.h>

#include "SSAOSceneRenderingStage.h"
#include "SSAOApplicationStage.h"


CPPEXPOSE_COMPONENT(SSAORenderingPipeline, gloperate::Stage)


SSAORenderingPipeline::SSAORenderingPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, canvasInterface(this)
, m_colorTextureStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment, "ColorTextureStage"))
, m_depthTextureStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment, "DepthTextureStage"))
, m_normalTextureStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment, "NormalTextureStage"))
, m_kernelStage(cppassist::make_unique<gloperate_glkernel::HemisphereDistributionKernelStage>(environment))
, m_noiseStage(cppassist::make_unique<gloperate_glkernel::NoiseKernelStage>(environment))
, m_renderingStage(cppassist::make_unique<SSAOSceneRenderingStage>(environment))
, m_colorTextureExtractionStage(cppassist::make_unique<gloperate::TextureFromRenderTargetExtractionStage>(environment))
, m_depthTextureExtractionStage(cppassist::make_unique<gloperate::TextureFromRenderTargetExtractionStage>(environment))
, m_normalTextureExtractionStage(cppassist::make_unique<gloperate::TextureFromRenderTargetExtractionStage>(environment))
, m_postprocessingStage(cppassist::make_unique<SSAOApplicationStage>(environment))
{
    addStage(m_colorTextureStage.get());
    m_colorTextureStage->format.setValue(gl::GL_RGBA);
    m_colorTextureStage->type.setValue(gl::GL_UNSIGNED_BYTE);
    m_colorTextureStage->internalFormat.setValue(gl::GL_RGBA8);
    m_colorTextureStage->size << canvasInterface.viewport;

    addStage(m_depthTextureStage.get());
    m_depthTextureStage->format.setValue(gl::GL_DEPTH_COMPONENT);
    m_depthTextureStage->type.setValue(gl::GL_UNSIGNED_BYTE);
    m_depthTextureStage->internalFormat.setValue(gl::GL_DEPTH_COMPONENT);
    m_depthTextureStage->size << canvasInterface.viewport;

    addStage(m_normalTextureStage.get());
    m_normalTextureStage->format.setValue(gl::GL_RGB);
    m_normalTextureStage->type.setValue(gl::GL_UNSIGNED_BYTE);
    m_normalTextureStage->internalFormat.setValue(gl::GL_RGB8);
    m_normalTextureStage->size << canvasInterface.viewport;

    addStage(m_kernelStage.get());
    m_kernelStage->kernelSize.setValue(16);

    addStage(m_noiseStage.get());
    m_noiseStage->dimensions.setValue(glm::ivec3(128, 128, 1));

    addStage(m_renderingStage.get());
    m_renderingStage->canvasInterface.backgroundColor << canvasInterface.backgroundColor;
    m_renderingStage->canvasInterface.viewport << canvasInterface.viewport;
    m_renderingStage->canvasInterface.frameCounter << canvasInterface.frameCounter;
    m_renderingStage->canvasInterface.timeDelta << canvasInterface.timeDelta;
    m_renderingStage->createInput("Color") << m_colorTextureStage->colorRenderTarget;
    m_renderingStage->createInput("Normal") << m_normalTextureStage->colorRenderTarget;
    m_renderingStage->createInput("Depth") << m_depthTextureStage->depthRenderTarget;

    addStage(m_colorTextureExtractionStage.get());
    m_colorTextureExtractionStage->colorRenderTarget << *m_renderingStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");

    addStage(m_depthTextureExtractionStage.get());
    m_depthTextureExtractionStage->depthRenderTarget << *m_renderingStage->createOutput<gloperate::DepthRenderTarget *>("DepthOut");

    addStage(m_normalTextureExtractionStage.get());
    m_normalTextureExtractionStage->colorRenderTarget << *m_renderingStage->createOutput<gloperate::ColorRenderTarget *>("NormalOut");

    addStage(m_postprocessingStage.get());
    m_postprocessingStage->renderInterface.viewport << canvasInterface.viewport;
    m_postprocessingStage->colorTexture << m_colorTextureExtractionStage->texture;
    m_postprocessingStage->normalTexture << m_normalTextureExtractionStage->texture;
    m_postprocessingStage->depthTexture << m_depthTextureExtractionStage->texture;
    m_postprocessingStage->ssaoKernel << m_kernelStage->texture;
    m_postprocessingStage->ssaoNoise << m_noiseStage->texture;
    m_postprocessingStage->projectionMatrix << m_renderingStage->projectionMatrix;
    m_postprocessingStage->normalMatrix << m_renderingStage->normalMatrix;
    m_postprocessingStage->createInput("Color") << *createInput<gloperate::ColorRenderTarget *>("Color");

    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *m_postprocessingStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
}

SSAORenderingPipeline::~SSAORenderingPipeline()
{
}
