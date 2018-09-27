
#include "MultiFrameRenderingPipeline.h"

#include <glbinding/gl/enum.h>

#include <globjects/NamedString.h>
#include <globjects/base/File.h>

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/TextureRenderTargetStage.h>
#include <gloperate/rendering/Quad.h>
#include <gloperate/rendering/Camera.h>
#include <gloperate/stages/base/ClearStage.h>
#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/base/RasterizationStage.h>
#include <gloperate/stages/base/RenderPassStage.h>
#include <gloperate/stages/base/TransformStage.h>

#include <gloperate-glkernel/stages/DiscDistributionKernelStage.h>
#include <gloperate-glkernel/stages/HemisphereDistributionKernelStage.h>
#include <gloperate-glkernel/stages/NoiseKernelStage.h>
#include <gloperate-glkernel/stages/TransparencyKernelStage.h>

#include "GeometryImporterStage.h"


CPPEXPOSE_COMPONENT(MultiFrameRenderingPipeline, gloperate::Stage)


MultiFrameRenderingPipeline::MultiFrameRenderingPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, name)
, canvasInterface(this)
, multiFrameCount("multiFrameCount", this, 1)
, camera("camera", this, nullptr)
, useAntialiasing("useAntialiasing", this, true)
, useDOF("useDOF", this, true)
, useSSAO("useSSAO", this, true)
, useTransparency("useTransparency", this, true)
, dofIntensity("dofIntensity", this, 0.01f)
, dofZFocus("dofZFocus", this, 0.1f)
, ssaoRadius("ssaoRadius", this, 0.35f)
, ssaoIntensity("ssaoIntensity", this, 0.7f)
, transparencyAlpha("transparencyAlpha", this, 0.65)
, m_colorTextureStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment, "ColorTextureStage"))
, m_depthTextureStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment, "DepthTextureStage"))
, m_normalTextureStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment, "NormalTextureStage"))
, m_subpixelStage(cppassist::make_unique<gloperate_glkernel::DiscDistributionKernelStage>(environment, "SubpixelShiftKernelStage"))
, m_dofShiftStage(cppassist::make_unique<gloperate_glkernel::DiscDistributionKernelStage>(environment, "DOFShiftKernelStage"))
, m_ssaoKernelStage(cppassist::make_unique<gloperate_glkernel::HemisphereDistributionKernelStage>(environment, "SSAOKernelStage"))
, m_noiseStage(cppassist::make_unique<gloperate_glkernel::NoiseKernelStage>(environment, "NoiseKernelStage"))
, m_transparencyKernelStage(cppassist::make_unique<gloperate_glkernel::TransparencyKernelStage>(environment, "TransparencyKernelStage"))
, m_renderGeometryStage(cppassist::make_unique<GeometryImporterStage>(environment, "GeometryImporterStage"))
, m_transformStage(cppassist::make_unique<gloperate::TransformStage>(environment, "TransformStage"))
, m_renderProgramStage(cppassist::make_unique<gloperate::ProgramStage>(environment, "RenderProgramStage"))
, m_renderPassStage(cppassist::make_unique<gloperate::RenderPassStage>(environment, "RenderPassStage"))
, m_renderClearStage(cppassist::make_unique<gloperate::ClearStage>(environment, "RenderClearStage"))
, m_renderRasterizationStage(cppassist::make_unique<gloperate::RasterizationStage>(environment, "RenderRasterizationStage"))
, m_postprocessingProgramStage(cppassist::make_unique<gloperate::ProgramStage>(environment, "PostprocessingProgramStage"))
, m_postprocessingPassStage(cppassist::make_unique<gloperate::RenderPassStage>(environment, "PostprocessingPassStage"))
, m_postprocessingClearStage(cppassist::make_unique<gloperate::ClearStage>(environment, "PostprocessingClearStage"))
, m_postprocessingRasterizationStage(cppassist::make_unique<gloperate::RasterizationStage>(environment, "PostprocessingRasterizationStage"))
, m_camera(cppassist::make_unique<gloperate::Camera>())
{
    camera = m_camera.get();
    camera->lookAt(glm::vec3(2.0f, 1.0f, 1.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    auto dataPath = gloperate::dataPath();

    addStage(m_colorTextureStage.get());
    m_colorTextureStage->format = gl::GL_RGBA;
    m_colorTextureStage->type = gl::GL_UNSIGNED_BYTE;
    m_colorTextureStage->internalFormat = gl::GL_RGBA8;
    m_colorTextureStage->size << canvasInterface.viewport;

    addStage(m_depthTextureStage.get());
    m_depthTextureStage->format = gl::GL_DEPTH_COMPONENT;
    m_depthTextureStage->type = gl::GL_UNSIGNED_BYTE;
    m_depthTextureStage->internalFormat = gl::GL_DEPTH_COMPONENT;
    m_depthTextureStage->size << canvasInterface.viewport;

    addStage(m_normalTextureStage.get());
    m_normalTextureStage->format = gl::GL_RGB;
    m_normalTextureStage->type = gl::GL_SHORT;
    m_normalTextureStage->internalFormat = gl::GL_RGB16_SNORM;
    m_normalTextureStage->size << canvasInterface.viewport;

    addStage(m_subpixelStage.get());
    m_subpixelStage->kernelSize << multiFrameCount;
    // subpixel shifts are scaled while rendering

    addStage(m_dofShiftStage.get());
    m_dofShiftStage->kernelSize << multiFrameCount;
    m_dofShiftStage->radius << dofIntensity;

    addStage(m_ssaoKernelStage.get());
    m_ssaoKernelStage->kernelSize = 1024;

    addStage(m_noiseStage.get());
    m_noiseStage->dimensions = glm::ivec3(64, 64, 64);

    addStage(m_transparencyKernelStage.get());
    m_transparencyKernelStage->kernelSize = glm::ivec2(256, 256);

    addStage(m_renderGeometryStage.get());
    m_renderGeometryStage->filePath = dataPath + "/gloperate/meshes/demos/multi_frame_demo.obj";

    addStage(m_transformStage.get());
    m_transformStage->translation = glm::vec3(-0.5f, 0.0f, 0.0f);

    addStage(m_renderProgramStage.get());
    *m_renderProgramStage->createInput<cppfs::FilePath>("vertexShader")   = dataPath + "/gloperate/shaders/demos/multi_frame_rendering.vert";
    *m_renderProgramStage->createInput<cppfs::FilePath>("fragmentShader") = dataPath + "/gloperate/shaders/demos/multi_frame_rendering.frag";

    addStage(m_renderPassStage.get());
    m_renderPassStage->drawable << m_renderGeometryStage->geometry;
    m_renderPassStage->program << m_renderProgramStage->program;
    m_renderPassStage->camera << camera;
    m_renderPassStage->modelMatrix << m_transformStage->modelMatrix;
    m_renderPassStage->createInput("currentFrame") << canvasInterface.frameCounter;
    m_renderPassStage->createInput("timeDelta") << canvasInterface.timeDelta;
    m_renderPassStage->createInput("dofShiftKernel") << m_dofShiftStage->texture;
    m_renderPassStage->createInput("useDOF") << useDOF;
    m_renderPassStage->createInput("dofZFocus") << dofZFocus;
    m_renderPassStage->createInput("subpixelShiftKernel") << m_subpixelStage->texture;
    m_renderPassStage->createInput("useAntialiasing") << useAntialiasing;
    m_renderPassStage->createInput("transparencyKernel") << m_transparencyKernelStage->texture;
    m_renderPassStage->createInput("useTransparency") << useTransparency;
    m_renderPassStage->createInput("transparencyAlpha") << transparencyAlpha;
    m_renderPassStage->createInput("viewport") << canvasInterface.viewport;

    addStage(m_renderClearStage.get());
    m_renderClearStage->createInput("Color") << m_colorTextureStage->colorRenderTarget;
    m_renderClearStage->createInput("ColorValue") = gloperate::Color(0.0f, 0.0f, 0.0f, 1.0f);
    m_renderClearStage->createInput("Normal") << m_normalTextureStage->colorRenderTarget;
    m_renderClearStage->createInput("NormalValue") = gloperate::Color(0.0f, 0.0f, 0.0f);
    m_renderClearStage->createInput("Depth") << m_depthTextureStage->depthRenderTarget;
    m_renderClearStage->createInput("DepthValue") = 1.0f;
    m_renderClearStage->renderInterface.viewport << canvasInterface.viewport;
    m_renderClearStage->createInput("ClearTrigger") << m_renderPassStage->renderPass;

    addStage(m_renderRasterizationStage.get());
    m_renderRasterizationStage->createInput("Color") << *m_renderClearStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
    m_renderRasterizationStage->createInput("Normal") << *m_renderClearStage->createOutput<gloperate::ColorRenderTarget *>("NormalOut");
    m_renderRasterizationStage->createInput("Depth") << *m_renderClearStage->createOutput<gloperate::DepthRenderTarget *>("DepthOut");
    m_renderRasterizationStage->renderInterface.viewport << canvasInterface.viewport;
    m_renderRasterizationStage->drawable << m_renderPassStage->renderPass;

    addStage(m_postprocessingProgramStage.get());
    *m_postprocessingProgramStage->createInput<cppfs::FilePath>("vertexShader")   = dataPath + "/gloperate/shaders/geometry/screenaligned.vert";
    *m_postprocessingProgramStage->createInput<cppfs::FilePath>("fragmentShader") = dataPath + "/gloperate/shaders/demos/multi_frame_postprocessing.frag";

    addStage(m_postprocessingPassStage.get());
    //m_postprocessingPassStage->drawable will be set in onContextInit
    m_postprocessingPassStage->program << m_postprocessingProgramStage->program;
    m_postprocessingPassStage->camera << camera;
    m_postprocessingPassStage->modelMatrix << m_transformStage->modelMatrix;
    m_postprocessingPassStage->depthTest = false;
    m_postprocessingPassStage->createInput("currentFrame") << canvasInterface.frameCounter;
    m_postprocessingPassStage->createInput("colorTexture") << m_colorTextureStage->texture;
    m_postprocessingPassStage->createInput("normalTexture") << m_normalTextureStage->texture;
    m_postprocessingPassStage->createInput("depthTexture") << m_depthTextureStage->texture;
    m_postprocessingPassStage->createInput("ssaoKernelTexture") << m_ssaoKernelStage->texture;
    m_postprocessingPassStage->createInput("ssaoNoiseTexture") << m_noiseStage->texture;
    m_postprocessingPassStage->createInput("useSSAO") << useSSAO;
    m_postprocessingPassStage->createInput("ssaoRadius") << ssaoRadius;
    m_postprocessingPassStage->createInput("ssaoIntensity") << ssaoIntensity;

    addStage(m_postprocessingClearStage.get());
    m_postprocessingClearStage->createInput("Color") << *createInput<gloperate::ColorRenderTarget *>("Color");
    m_postprocessingClearStage->createInput("ColorValue") << canvasInterface.backgroundColor;
    m_postprocessingClearStage->renderInterface.viewport << canvasInterface.viewport;
    m_postprocessingClearStage->createInput("ClearTrigger") << *m_renderRasterizationStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");

    addStage(m_postprocessingRasterizationStage.get());
    m_postprocessingRasterizationStage->createInput("Color") << *m_postprocessingClearStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
    m_postprocessingRasterizationStage->renderInterface.viewport << canvasInterface.viewport;
    m_postprocessingRasterizationStage->drawable << m_postprocessingPassStage->renderPass;

    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *m_postprocessingRasterizationStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
}

MultiFrameRenderingPipeline::~MultiFrameRenderingPipeline()
{
}

void MultiFrameRenderingPipeline::onContextInit(gloperate::AbstractGLContext * context)
{
    Pipeline::onContextInit(context);

    m_ssaoNamedString   = globjects::NamedString::create("/gloperate/shaders/lighting/ssao.glsl", new globjects::File(gloperate::dataPath() + "/gloperate/shaders/lighting/ssao.glsl"));
    m_randomNamedString = globjects::NamedString::create("/gloperate/shaders/util/random.glsl",   new globjects::File(gloperate::dataPath() + "/gloperate/shaders/util/random.glsl"));
    m_quad = cppassist::make_unique<gloperate::Quad>();

    m_postprocessingPassStage->drawable = m_quad.get();
}

void MultiFrameRenderingPipeline::onContextDeinit(gloperate::AbstractGLContext * context)
{
    m_quad.reset();
    m_randomNamedString.reset();
    m_ssaoNamedString.reset();
    m_camera.reset();

    Pipeline::onContextDeinit(context);
}
