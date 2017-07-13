
#include "MultiFrameRenderingPipeline.h"

#include <cppassist/memory/make_unique.h>

#include <glbinding/gl/enum.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/TextureRenderTargetStage.h>
#include <gloperate/stages/base/TextureFromRenderTargetExtractionStage.h>

#include <gloperate-glkernel/stages/DiscDistributionKernelStage.h>
#include <gloperate-glkernel/stages/HemisphereDistributionKernelStage.h>
#include <gloperate-glkernel/stages/NoiseKernelStage.h>
#include <gloperate-glkernel/stages/TransparencyKernelStage.h>

#include "MultiFrameSceneRenderingStage.h"
#include "MultiFramePostprocessingStage.h"


CPPEXPOSE_COMPONENT(MultiFrameRenderingPipeline, gloperate::Stage)


MultiFrameRenderingPipeline::MultiFrameRenderingPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, canvasInterface(this)
, multiFrameCount("multiFrameCount", this, 1)
, m_colorTextureStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment, "ColorTextureStage"))
, m_depthTextureStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment, "DepthTextureStage"))
, m_normalTextureStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment, "NormalTextureStage"))
, m_subpixelStage(cppassist::make_unique<gloperate_glkernel::DiscDistributionKernelStage>(environment))
, m_dofShiftStage(cppassist::make_unique<gloperate_glkernel::DiscDistributionKernelStage>(environment))
, m_ssaoKernelStage(cppassist::make_unique<gloperate_glkernel::HemisphereDistributionKernelStage>(environment))
, m_noiseStage(cppassist::make_unique<gloperate_glkernel::NoiseKernelStage>(environment))
, m_transparencyKernelStage(cppassist::make_unique<gloperate_glkernel::TransparencyKernelStage>(environment))
, m_renderingStage(cppassist::make_unique<MultiFrameSceneRenderingStage>(environment))
, m_postprocessingStage(cppassist::make_unique<MultiFramePostprocessingStage>(environment))
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

    addStage(m_subpixelStage.get());
    m_subpixelStage->kernelSize << multiFrameCount;
    // subpixel shifts are scaled in rendering stage

    addStage(m_dofShiftStage.get());
    m_dofShiftStage->kernelSize << multiFrameCount;
    m_dofShiftStage->radius = 0.01f;

    addStage(m_ssaoKernelStage.get());
    m_ssaoKernelStage->kernelSize.setValue(16);

    addStage(m_noiseStage.get());
    m_noiseStage->dimensions.setValue(glm::ivec3(64, 64, 64));

    addStage(m_transparencyKernelStage.get());
    m_transparencyKernelStage->kernelSize.setValue(glm::ivec2(256, 256));

    addStage(m_renderingStage.get());
    m_renderingStage->canvasInterface.backgroundColor << canvasInterface.backgroundColor;
    m_renderingStage->canvasInterface.viewport << canvasInterface.viewport;
    m_renderingStage->canvasInterface.frameCounter << canvasInterface.frameCounter;
    m_renderingStage->canvasInterface.timeDelta << canvasInterface.timeDelta;
    m_renderingStage->createInput("Color") << m_colorTextureStage->colorRenderTarget;
    m_renderingStage->createInput("Depth") << m_depthTextureStage->depthRenderTarget;
    m_renderingStage->createInput("Normal") << m_normalTextureStage->colorRenderTarget;
    m_renderingStage->subpixelShiftKernel << m_subpixelStage->kernel;
    m_renderingStage->dofShiftKernel << m_dofShiftStage->kernel;
    m_renderingStage->transparencyKernelTexture << m_transparencyKernelStage->texture;

    addStage(m_postprocessingStage.get());
    m_postprocessingStage->canvasInterface.backgroundColor << canvasInterface.backgroundColor;
    m_postprocessingStage->canvasInterface.viewport << canvasInterface.viewport;
    m_postprocessingStage->canvasInterface.frameCounter << canvasInterface.frameCounter;
    m_postprocessingStage->canvasInterface.timeDelta << canvasInterface.timeDelta;
    m_postprocessingStage->createInput("Color") << *createInput<gloperate::ColorRenderTarget *>("Color");
    m_postprocessingStage->colorTexture << m_colorTextureStage->texture;
    m_postprocessingStage->normalTexture << m_normalTextureStage->texture;
    m_postprocessingStage->depthTexture << m_depthTextureStage->texture;
    m_postprocessingStage->ssaoKernel << m_ssaoKernelStage->texture;
    m_postprocessingStage->ssaoNoise << m_noiseStage->texture;
    m_postprocessingStage->projectionMatrix << m_renderingStage->projectionMatrix;
    m_postprocessingStage->normalMatrix << m_renderingStage->normalMatrix;

    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *m_postprocessingStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
}

MultiFrameRenderingPipeline::~MultiFrameRenderingPipeline()
{
}
