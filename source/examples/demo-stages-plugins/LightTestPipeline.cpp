
#include "LightTestPipeline.h"

#include <glbinding/gl/enum.h>

#include <globjects/NamedString.h>
#include <globjects/base/File.h>

#include <gloperate/gloperate.h>
#include <gloperate/rendering/ColorRenderTarget.h>
#include <gloperate/rendering/ShapeType.h>
#include <gloperate/rendering/Camera.h>
#include <gloperate/stages/base/TextureRenderTargetStage.h>
#include <gloperate/stages/base/TimerStage.h>
#include <gloperate/stages/base/ClearStage.h>
#include <gloperate/stages/base/ShapeStage.h>
#include <gloperate/stages/base/TransformStage.h>
#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/base/RenderPassStage.h>
#include <gloperate/stages/base/RasterizationStage.h>
#include <gloperate/stages/lights/LightCreationStage.h>
#include <gloperate/stages/lights/LightBufferTextureStage.h>


CPPEXPOSE_COMPONENT(LightTestPipeline, gloperate::Stage)


LightTestPipeline::LightTestPipeline(gloperate::Environment * environment, const std::string & name)
: Pipeline(environment, "LightTestPipeline", name)
, canvasInterface(this)
, glossiness("glossiness", this)
, lightType1("lightType1", this, 0)
, lightType2("lightType2", this, 1)
, lightType3("lightType3", this, 4)
, lightColor1("lightColor1", this, glm::vec3(0.5, 0, 0))
, lightColor2("lightColor2", this, glm::vec3(0.125, 0.125, 0.125))
, lightColor3("lightColor3", this, glm::vec3(1, 1, 1))
, lightPos1("lightPos1", this, glm::vec3(-1, 0, 2))
, lightPos2("lightPos2", this, glm::vec3(0, 0.6, 0))
, lightPos3("lightPos3", this, glm::vec3(1.0, 0.8, 1.0))
, lightAttenuation1("lightAttenuation1", this, glm::vec3(1, 0.2f, 0.01f))
, lightAttenuation2("lightAttenuation2", this, glm::vec3(1, 0.2f, 0.01f))
, lightAttenuation3("lightAttenuation3", this, glm::vec3(1, 0.2f, 0.01f))
, m_lightDefStage1(cppassist::make_unique<gloperate::LightCreationStage>(environment))
, m_lightDefStage2(cppassist::make_unique<gloperate::LightCreationStage>(environment))
, m_lightDefStage3(cppassist::make_unique<gloperate::LightCreationStage>(environment))
, m_lightAccumulationStage(cppassist::make_unique<gloperate::LightBufferTextureStage>(environment))
, m_depthBufferStage(cppassist::make_unique<gloperate::TextureRenderTargetStage>(environment))
, m_clearStage(cppassist::make_unique<gloperate::ClearStage>(environment))
, m_shapeStage(cppassist::make_unique<gloperate::ShapeStage>(environment))
, m_transformStage(cppassist::make_unique<gloperate::TransformStage>(environment))
, m_timerStage(cppassist::make_unique<gloperate::TimerStage>(environment))
, m_programStage(cppassist::make_unique<gloperate::ProgramStage>(environment))
, m_renderPassStage(cppassist::make_unique<gloperate::RenderPassStage>(environment))
, m_rasterizationStage(cppassist::make_unique<gloperate::RasterizationStage>(environment))
, m_camera(cppassist::make_unique<gloperate::Camera>(glm::vec3(0.0f, 2.0f, 3.0f)))
{
    const auto dataPath = gloperate::dataPath();

    const cppexpose::VariantMap typeOptions{{
        {"type", "int"}, // HACK: override auto-assigned value "int32" to show editor
        {"asSpinBox", true},
        {"minimumValue", 0},
        {"maximumValue", 4},
        {"decimals", 0},
        {"stepSize", 1}
    }};

    lightType1.setOptions(typeOptions);
    lightType2.setOptions(typeOptions);
    lightType3.setOptions(typeOptions);

    addStage(m_lightDefStage1.get());
    m_lightDefStage1->color << lightColor1;
    m_lightDefStage1->position << lightPos1;
    m_lightDefStage1->type << lightType1;
    m_lightDefStage1->attenuationCoefficients << lightAttenuation1;
    addStage(m_lightDefStage2.get());
    m_lightDefStage2->color << lightColor2;
    m_lightDefStage2->position << lightPos2;
    m_lightDefStage2->type << lightType2;
    m_lightDefStage2->attenuationCoefficients << lightAttenuation2;
    addStage(m_lightDefStage3.get());
    m_lightDefStage3->color << lightColor3;
    m_lightDefStage3->position << lightPos3;
    m_lightDefStage3->type << lightType3;
    m_lightDefStage3->attenuationCoefficients << lightAttenuation3;

    addStage(m_lightAccumulationStage.get());
    *(m_lightAccumulationStage->createLightInput()) << m_lightDefStage1->light;
    *(m_lightAccumulationStage->createLightInput()) << m_lightDefStage2->light;
    *(m_lightAccumulationStage->createLightInput()) << m_lightDefStage3->light;

    addStage(m_timerStage.get());
    m_timerStage->timeDelta << canvasInterface.timeDelta;

    addStage(m_depthBufferStage.get());
    m_depthBufferStage->size << canvasInterface.viewport;
    m_depthBufferStage->type = gl::GL_UNSIGNED_BYTE;
    m_depthBufferStage->format = gl::GL_DEPTH_COMPONENT;
    m_depthBufferStage->internalFormat = gl::GL_DEPTH_COMPONENT;

    addStage(m_programStage.get());
    *m_programStage->createInput<cppassist::FilePath>("vertexShader")   = dataPath + "/gloperate/shaders/demos/lights.vert";
    *m_programStage->createInput<cppassist::FilePath>("geometryShader") = dataPath + "/gloperate/shaders/demos/lights.geom";
    *m_programStage->createInput<cppassist::FilePath>("fragmentShader") = dataPath + "/gloperate/shaders/demos/lights.frag";

    addStage(m_shapeStage.get());
    m_shapeStage->shapeType = gloperate::ShapeType::Box;
    m_shapeStage->width     = 1.0f;
    m_shapeStage->height    = 1.0f;
    m_shapeStage->depth     = 1.0f;

    addStage(m_transformStage.get());
    m_transformStage->rotationAngle << m_timerStage->virtualTime;

    addStage(m_renderPassStage.get());
    m_renderPassStage->drawable << m_shapeStage->drawable;
    m_renderPassStage->program << m_programStage->program;
    m_renderPassStage->camera = m_camera.get();
    m_renderPassStage->modelMatrix << m_transformStage->modelMatrix;
    m_renderPassStage->createInput("glossiness") << glossiness;
    m_renderPassStage->createInput("colorTypeData") << m_lightAccumulationStage->colorTypeData;
    m_renderPassStage->createInput("positionData") << m_lightAccumulationStage->positionData;
    m_renderPassStage->createInput("attenuationData") << m_lightAccumulationStage->attenuationData;

    addStage(m_clearStage.get());
    m_clearStage->createInput("Color") << *createInput<gloperate::ColorRenderTarget *>("Color");
    m_clearStage->createInput("Depth") << m_depthBufferStage->depthRenderTarget;
    m_clearStage->createInput("ColorValue") << canvasInterface.backgroundColor;
    m_clearStage->createInput("DepthValue") = 1.0f;
    m_clearStage->renderInterface.viewport << canvasInterface.viewport;
    m_clearStage->createInput("clearTrigger") << m_renderPassStage->renderPass;

    addStage(m_rasterizationStage.get());
    m_rasterizationStage->createInput("Color") << *m_clearStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
    m_rasterizationStage->createInput("Depth") << *m_clearStage->createOutput<gloperate::DepthRenderTarget *>("DepthOut");
    m_rasterizationStage->renderInterface.viewport << canvasInterface.viewport;
    m_rasterizationStage->drawable << m_renderPassStage->renderPass;

    *createOutput<gloperate::ColorRenderTarget *>("ColorOut") << *m_rasterizationStage->createOutput<gloperate::ColorRenderTarget *>("ColorOut");
}

LightTestPipeline::~LightTestPipeline()
{
}

void LightTestPipeline::onContextInit(gloperate::AbstractGLContext * context)
{
    const auto dataFolderPath = gloperate::dataPath();
    m_lightProcessingString        = globjects::NamedString::create("/gloperate/shaders/lighting/lightprocessing.glsl", new globjects::File(dataFolderPath + "/gloperate/shaders/lighting/lightprocessing.glsl"));
    m_lightProcessingDiffuseString = globjects::NamedString::create("/gloperate/shaders/lighting/lightprocessing_diffuse.glsl", new globjects::File(dataFolderPath + "/gloperate/shaders/lighting/lightprocessing_diffuse.glsl"));
    m_lightProcessingPhongString   = globjects::NamedString::create("/gloperate/shaders/lighting/lightprocessing_phong.glsl", new globjects::File(dataFolderPath + "/gloperate/shaders/lighting/lightprocessing_phong.glsl"));

    Pipeline::onContextInit(context);
}

void LightTestPipeline::onContextDeinit(gloperate::AbstractGLContext * context)
{
    Pipeline::onContextDeinit(context);

    m_lightProcessingDiffuseString.reset();
    m_lightProcessingPhongString.reset();
    m_lightProcessingString.reset();
}
