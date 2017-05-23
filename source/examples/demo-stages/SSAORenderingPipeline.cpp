
#include "SSAORenderingPipeline.h"

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/FramebufferStage.h>
#include <gloperate/stages/base/TextureStage.h>

#include <gloperate-glkernel/stages/HemisphereDistributionKernelStage.h>
#include <gloperate-glkernel/stages/NoiseKernelStage.h>

#include <glbinding/gl/enum.h>

#include "SSAOSceneRenderingStage.h"
#include "SSAOApplicationStage.h"


CPPEXPOSE_COMPONENT(SSAORenderingPipeline, gloperate::Stage)


SSAORenderingPipeline::SSAORenderingPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, m_colorTextureStage(cppassist::make_unique<gloperate::TextureStage>(environment, "ColorTextureStage"))
, m_depthTextureStage(cppassist::make_unique<gloperate::TextureStage>(environment, "DepthTextureStage"))
, m_normalTextureStage(cppassist::make_unique<gloperate::TextureStage>(environment, "NormalTextureStage"))
, m_fboStage(cppassist::make_unique<gloperate::FramebufferStage>(environment))
, m_kernelStage(cppassist::make_unique<gloperate_glkernel::HemisphereDistributionKernelStage>(environment))
, m_noiseStage(cppassist::make_unique<gloperate_glkernel::NoiseKernelStage>(environment))
, m_renderingStage(cppassist::make_unique<SSAOSceneRenderingStage>(environment))
, m_postprocessingStage(cppassist::make_unique<SSAOApplicationStage>(environment))
{
    addStage(m_colorTextureStage.get());
    m_colorTextureStage->format.setValue(gl::GL_RGBA);
    m_colorTextureStage->type.setValue(gl::GL_UNSIGNED_BYTE);
    m_colorTextureStage->internalFormat.setValue(gl::GL_RGBA8);
    m_colorTextureStage->size << renderInterface.deviceViewport;

    addStage(m_depthTextureStage.get());
    m_depthTextureStage->format.setValue(gl::GL_DEPTH_COMPONENT);
    m_depthTextureStage->type.setValue(gl::GL_UNSIGNED_BYTE);
    m_depthTextureStage->internalFormat.setValue(gl::GL_DEPTH_COMPONENT);
    m_depthTextureStage->size << renderInterface.deviceViewport;

    addStage(m_normalTextureStage.get());
    m_normalTextureStage->format.setValue(gl::GL_RGB);
    m_normalTextureStage->type.setValue(gl::GL_UNSIGNED_BYTE);
    m_normalTextureStage->internalFormat.setValue(gl::GL_RGB8);
    m_normalTextureStage->size << renderInterface.deviceViewport;

    addStage(m_fboStage.get());
    m_fboStage->colorTexture << m_colorTextureStage->renderTarget;
    m_fboStage->depthTexture << m_depthTextureStage->renderTarget;
    *(m_fboStage->createInput<gloperate::RenderTarget *>("Normal Texture")) << m_normalTextureStage->renderTarget;

    addStage(m_kernelStage.get());
    m_kernelStage->kernelSize.setValue(16);

    addStage(m_noiseStage.get());
    m_noiseStage->dimensions.setValue(glm::ivec3(128, 128, 1));

    addStage(m_renderingStage.get());
    m_renderingStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_renderingStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_renderingStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_renderingStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_renderingStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_renderingStage->renderInterface.targetFBO << m_fboStage->fbo;

    addStage(m_postprocessingStage.get());
    m_postprocessingStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_postprocessingStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_postprocessingStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_postprocessingStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_postprocessingStage->renderInterface.targetFBO << renderInterface.targetFBO;
    m_postprocessingStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_postprocessingStage->colorTexture << m_colorTextureStage->texture;
    m_postprocessingStage->normalTexture << m_normalTextureStage->texture;
    m_postprocessingStage->depthTexture << m_depthTextureStage->texture;
    m_postprocessingStage->ssaoKernel << m_kernelStage->texture;
    m_postprocessingStage->ssaoNoise << m_noiseStage->texture;
    m_postprocessingStage->projectionMatrix << m_renderingStage->projectionMatrix;
    m_postprocessingStage->normalMatrix << m_renderingStage->normalMatrix;
    m_postprocessingStage->sceneRendered << m_renderingStage->renderInterface.rendered;

    renderInterface.rendered << m_postprocessingStage->renderInterface.rendered;
}

SSAORenderingPipeline::~SSAORenderingPipeline()
{
}
