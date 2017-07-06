
#include "MultiFrameRenderingPipeline.h"

#include <cppassist/memory/make_unique.h>
#include <cppassist/fs/FilePath.h>

#include <glbinding/gl/enum.h>

#include <globjects/NamedString.h>
#include <globjects/base/File.h>

#include <gloperate/gloperate.h>
#include <gloperate/rendering/ScreenAlignedQuad.h>
#include <gloperate/rendering/Camera.h>
#include <gloperate/stages/base/FramebufferStage.h>
#include <gloperate/stages/base/TextureStage.h>
#include <gloperate/stages/base/ClearStage.h>
#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/base/RenderPassStage.h>
#include <gloperate/stages/base/RasterizationStage.h>

#include <gloperate-glkernel/stages/DiscDistributionKernelStage.h>
#include <gloperate-glkernel/stages/HemisphereDistributionKernelStage.h>
#include <gloperate-glkernel/stages/NoiseKernelStage.h>
#include <gloperate-glkernel/stages/TransparencyKernelStage.h>

#include "GeometryImporterStage.h"


CPPEXPOSE_COMPONENT(MultiFrameRenderingPipeline, gloperate::Stage)


MultiFrameRenderingPipeline::MultiFrameRenderingPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, renderInterface(this)
, multiFrameCount("multiFrameCount", this, 1)
, m_colorTextureStage(cppassist::make_unique<gloperate::TextureStage>(environment, "ColorTextureStage"))
, m_depthTextureStage(cppassist::make_unique<gloperate::TextureStage>(environment, "DepthTextureStage"))
, m_normalTextureStage(cppassist::make_unique<gloperate::TextureStage>(environment, "NormalTextureStage"))
, m_fboStage(cppassist::make_unique<gloperate::FramebufferStage>(environment))
, m_subpixelStage(cppassist::make_unique<gloperate_glkernel::DiscDistributionKernelStage>(environment))
, m_dofShiftStage(cppassist::make_unique<gloperate_glkernel::DiscDistributionKernelStage>(environment))
, m_ssaoKernelStage(cppassist::make_unique<gloperate_glkernel::HemisphereDistributionKernelStage>(environment))
, m_noiseStage(cppassist::make_unique<gloperate_glkernel::NoiseKernelStage>(environment))
, m_transparencyKernelStage(cppassist::make_unique<gloperate_glkernel::TransparencyKernelStage>(environment))
//, m_renderingStage(cppassist::make_unique<MultiFrameSceneRenderingStage>(environment))
//, m_postprocessingStage(cppassist::make_unique<MultiFramePostprocessingStage>(environment))
, m_renderGeometryStage(cppassist::make_unique<GeometryImporterStage>(environment))
, m_renderProgramStage(cppassist::make_unique<gloperate::ProgramStage>(environment, "RenderProgramStage"))
, m_renderPassStage(cppassist::make_unique<gloperate::RenderPassStage>(environment, "RenderPassStage"))
, m_renderClearStage(cppassist::make_unique<gloperate::ClearStage>(environment, "RenderClearStage"))
, m_renderRasterizationStage(cppassist::make_unique<gloperate::RasterizationStage>(environment, "RenderRasterizationStage"))
, m_postprocessingProgramStage(cppassist::make_unique<gloperate::ProgramStage>(environment, "PostprocessingProgramStage"))
, m_postprocessingPassStage(cppassist::make_unique<gloperate::RenderPassStage>(environment, "PostprocessingPassStage"))
, m_postprocessingClearStage(cppassist::make_unique<gloperate::ClearStage>(environment, "PostprocessingClearStage"))
, m_postprocessingRasterizationStage(cppassist::make_unique<gloperate::RasterizationStage>(environment, "PostprocessingRasterizationStage"))
{
    auto dataPath = gloperate::dataPath();

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
    *(m_fboStage->createInput<gloperate::RenderTarget *>("NormalTexture")) << m_normalTextureStage->renderTarget;

    addStage(m_subpixelStage.get());
    m_subpixelStage->kernelSize << multiFrameCount;
    // subpixel shifts are scaled while rendering

    addStage(m_dofShiftStage.get());
    m_dofShiftStage->kernelSize << multiFrameCount;
    m_dofShiftStage->radius = 0.01f;

    addStage(m_ssaoKernelStage.get());
    m_ssaoKernelStage->kernelSize.setValue(16);

    addStage(m_noiseStage.get());
    m_noiseStage->dimensions.setValue(glm::ivec3(64, 64, 64));

    addStage(m_transparencyKernelStage.get());
    m_transparencyKernelStage->kernelSize.setValue(glm::ivec2(256, 256));

    /*
    addStage(m_renderingStage.get());
    m_renderingStage->renderInterface.backgroundColor << renderInterface.backgroundColor;
    m_renderingStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_renderingStage->renderInterface.virtualViewport << renderInterface.virtualViewport;
    m_renderingStage->renderInterface.frameCounter << renderInterface.frameCounter;
    m_renderingStage->renderInterface.timeDelta << renderInterface.timeDelta;
    m_renderingStage->renderInterface.targetFBO << m_fboStage->fbo;
    m_renderingStage->subpixelShiftKernel << m_subpixelStage->kernel;
    m_renderingStage->dofShiftKernel << m_dofShiftStage->kernel;
    m_renderingStage->transparencyKernelTexture << m_transparencyKernelStage->texture;
    */

    addStage(m_renderGeometryStage.get());
    m_renderGeometryStage->filePath = dataPath + "/gloperate/meshes/demos/multi_frame_demo.obj";

    addStage(m_renderProgramStage.get());
    *m_renderProgramStage->createInput<cppassist::FilePath>("vertexShader")   = dataPath + "/gloperate/shaders/demos/multi_frame_rendering.vert";
    *m_renderProgramStage->createInput<cppassist::FilePath>("fragmentShader") = dataPath + "/gloperate/shaders/demos/multi_frame_rendering.frag";

    addStage(m_renderPassStage.get());
    m_renderPassStage->drawable << m_renderGeometryStage->geometry;
    m_renderPassStage->program << m_renderProgramStage->program;
    //m_renderPassStage->camera is set in onContextInit
    m_renderPassStage->createInput("currentFrame") << renderInterface.frameCounter;
    m_renderPassStage->createInput("dofShiftKernel") << m_dofShiftStage->texture;
    m_renderPassStage->createInput("subpixelShiftKernel") << m_subpixelStage->texture;
    m_renderPassStage->createInput("transparencyKernel") << m_transparencyKernelStage->texture;
    m_renderPassStage->createInput("viewport") << renderInterface.deviceViewport;

    addStage(m_renderClearStage.get());
    m_renderClearStage->renderInterface.targetFBO << m_fboStage->fbo;
    m_renderClearStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_renderClearStage->renderInterface.backgroundColor = gloperate::Color(0.0f, 0.0f, 0.0f, 1.0f);

    addStage(m_renderRasterizationStage.get());
    m_renderRasterizationStage->renderInterface.targetFBO << m_renderClearStage->fboOut;
    m_renderRasterizationStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_renderRasterizationStage->drawable << m_renderPassStage->renderPass;

    /*
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
    m_postprocessingStage->ssaoKernel << m_ssaoKernelStage->texture;
    m_postprocessingStage->ssaoNoise << m_noiseStage->texture;
    m_postprocessingStage->projectionMatrix << m_renderingStage->projectionMatrix;
    m_postprocessingStage->normalMatrix << m_renderingStage->normalMatrix;
    m_postprocessingStage->sceneRendered << m_renderingStage->renderInterface.rendered;
    */

    addStage(m_postprocessingProgramStage.get());
    *m_postprocessingProgramStage->createInput<cppassist::FilePath>("vertexShader")   = dataPath + "/gloperate/shaders/geometry/screenaligned.vert";
    *m_postprocessingProgramStage->createInput<cppassist::FilePath>("fragmentShader") = dataPath + "/gloperate/shaders/demos/multi_frame_postprocessing.frag";

    addStage(m_postprocessingPassStage.get());
    //m_postprocessingPassStage->drawable will be set in onContextInit
    m_postprocessingPassStage->program << m_postprocessingProgramStage->program;
    m_postprocessingPassStage->depthTest = false;
    m_postprocessingPassStage->createInput("currentFrame") << renderInterface.frameCounter;
    m_postprocessingPassStage->createInput("colorTexture") << m_colorTextureStage->texture;
    m_postprocessingPassStage->createInput("normalTexture") << m_normalTextureStage->texture;
    m_postprocessingPassStage->createInput("depthTexture") << m_depthTextureStage->texture;
    m_postprocessingPassStage->createInput("ssaoKernelTexture") << m_ssaoKernelStage->texture;
    m_postprocessingPassStage->createInput("ssaoNoiseTexture") << m_noiseStage->texture;
    //m_postprocessingPassStage->createInput("projectionMatrix") will be set in onContextInit
    //m_postprocessingPassStage->createInput("projectionInverseMatrix") will be set in onContextInit
    //m_postprocessingPassStage->createInput("normalMatrix") will be set in onContextInit

    addStage(m_postprocessingClearStage.get());
    m_postprocessingClearStage->renderInterface.targetFBO << m_renderRasterizationStage->fboOut;
    m_postprocessingClearStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_postprocessingClearStage->renderInterface.backgroundColor << renderInterface.backgroundColor;

    addStage(m_postprocessingRasterizationStage.get());
    m_postprocessingRasterizationStage->renderInterface.targetFBO << m_postprocessingClearStage->fboOut;
    m_postprocessingRasterizationStage->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_postprocessingRasterizationStage->drawable << m_postprocessingPassStage->renderPass;

    //renderInterface.rendered << m_postprocessingStage->renderInterface.rendered;
    renderInterface.rendered << m_postprocessingRasterizationStage->renderInterface.rendered;
}

MultiFrameRenderingPipeline::~MultiFrameRenderingPipeline()
{
}

void MultiFrameRenderingPipeline::onContextInit(gloperate::AbstractGLContext * context)
{
    if (m_camera)
        return;

    m_camera = cppassist::make_unique<gloperate::Camera>(glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    m_ssaoNamedString = globjects::NamedString::create("/gloperate/shaders/lighting/ssao.glsl", new globjects::File(gloperate::dataPath() + "/gloperate/shaders/lighting/ssao.glsl"));
    m_screenAlignedQuad = cppassist::make_unique<gloperate::ScreenAlignedQuad>();

    m_renderPassStage->camera = m_camera.get();
    m_postprocessingPassStage->drawable = m_screenAlignedQuad.get();
    m_postprocessingPassStage->camera = m_camera.get(); // provide necessary matrices for postprocessing calculations

    Pipeline::onContextInit(context);
}

void MultiFrameRenderingPipeline::onContextDeinit(gloperate::AbstractGLContext * context)
{
    Pipeline::onContextDeinit(context);

    m_screenAlignedQuad.reset();
    m_ssaoNamedString.reset();
    m_camera.reset();
}
