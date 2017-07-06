
#include "ColorGradientDemo.h"

#include <gloperate/gloperate.h>
#include <gloperate/stages/base/ProgramStage.h>
#include <gloperate/stages/base/RenderPassStage.h>
#include <gloperate/stages/base/RasterizationStage.h>
#include <gloperate/stages/base/ClearStage.h>
#include <gloperate/stages/base/ShapeStage.h>
#include <gloperate/stages/base/TransformStage.h>
#include <gloperate/stages/base/ColorGradientStage.h>
#include <gloperate/stages/base/ColorGradientSelectionStage.h>
#include <gloperate/stages/base/ColorGradientTextureStage.h>
#include <gloperate/stages/navigation/TrackballStage.h>
#include <gloperate/rendering/ColorGradientList.h>
#include <gloperate/rendering/Shape.h>
#include <gloperate/rendering/Image.h>


CPPEXPOSE_COMPONENT(ColorGradientDemo, gloperate::Stage)


using namespace cppexpose;
using namespace gloperate;


ColorGradientDemo::ColorGradientDemo(Environment * environment, const std::string & name)
: Pipeline(environment, "ColorGradientDemo", name)
, renderInterface(this)
, colors("colors", this, dataPath() + "/gloperate/gradients/colorbrewer.json")
, gradient("gradient", this, "Set1-5")
, value("value", this, 0.5f)
, m_trackball(cppassist::make_unique<TrackballStage>(environment, "Trackball"))
, m_shape(cppassist::make_unique<ShapeStage>(environment, "Shape"))
, m_colorGradientLoading(cppassist::make_unique<ColorGradientStage>(environment, "ColorGradientsLoading"))
, m_colorGradientTexture(cppassist::make_unique<ColorGradientTextureStage>(environment, "ColorGradientsTexture"))
, m_colorGradientSelection(cppassist::make_unique<ColorGradientSelectionStage>(environment, "ColorGradientsSelection"))
, m_shapeTransform(cppassist::make_unique<TransformStage>(environment, "ShapeTransform"))
, m_shapeProgram(cppassist::make_unique<ProgramStage>(environment, "ShapeProgram"))
, m_shapeRenderPass(cppassist::make_unique<RenderPassStage>(environment, "ShapeRenderPass"))
, m_clear(cppassist::make_unique<ClearStage>(environment, "Clear"))
, m_shapeRasterization(cppassist::make_unique<RasterizationStage>(environment, "ShapeRasterization"))
{
    // Get data path
    std::string dataPath = gloperate::dataPath();

    // Setup parameters
    value.setOption("minimumValue", 0.0f);
    value.setOption("maximumValue", 1.0f);
    value.setOption("updateOnDrag", true);
    value.valueChanged.connect([this] (const float & value)
    {
        float v = 0.2 + 0.8 * value;
        m_shapeTransform->scale = glm::vec3(v, v, v);
    });

    // Trackball stage
    addStage(m_trackball.get());
    m_trackball->viewport << renderInterface.deviceViewport;

    // Shape stage
    addStage(m_shape.get());
    m_shape->shapeType = ShapeType::Box;
    m_shape->width     = 1.0f;
    m_shape->height    = 1.0f;
    m_shape->depth     = 1.0f;
    m_shape->texCoords = false;

    // Color gradient load stage
    addStage(m_colorGradientLoading.get());
    m_colorGradientLoading->filePath << colors;
    m_colorGradientLoading->gradients.valueChanged.connect([this] (const gloperate::ColorGradientList * gradients)
    {
        gradient.setOption("choices", cppexpose::Variant::fromVector(gradients->names()));
        gradient.setOption("pixmaps", cppexpose::Variant::fromVector(gradients->pixmaps({ 80, 20 })));
    });

    // Color gradients texture stage
    addStage(m_colorGradientTexture.get());
    m_colorGradientTexture->gradients << m_colorGradientLoading->gradients;
    m_colorGradientTexture->textureWidth = 128;

    // Color gradient selection stage
    addStage(m_colorGradientSelection.get());
    m_colorGradientSelection->gradients << m_colorGradientLoading->gradients;
    m_colorGradientSelection->name << gradient;

    // Transform stage for shape
    addStage(m_shapeTransform.get());
    value.valueChanged(value.value());

    // Program stage for shape
    addStage(m_shapeProgram.get());
    *m_shapeProgram->createInput<cppassist::FilePath>("vertexShader")   = dataPath + "/gloperate/shaders/demos/gradient.vert";
    *m_shapeProgram->createInput<cppassist::FilePath>("fragmentShader") = dataPath + "/gloperate/shaders/demos/gradient.frag";

    // Render pass stage for shape
    addStage(m_shapeRenderPass.get());
    m_shapeRenderPass->culling = false;
    m_shapeRenderPass->drawable << m_shape->drawable;
    m_shapeRenderPass->program << m_shapeProgram->program;
    m_shapeRenderPass->camera << m_trackball->camera;
    m_shapeRenderPass->modelMatrix << m_shapeTransform->modelMatrix;
    m_shapeRenderPass->createInput("gradientTexture") << m_colorGradientTexture->texture;
    m_shapeRenderPass->createInput("gradientIndex") << m_colorGradientSelection->index;
    m_shapeRenderPass->createInput("value") << this->value;

    // Clear stage
    addStage(m_clear.get());
    m_clear->renderInterface.targetFBO << renderInterface.targetFBO;
    m_clear->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_clear->renderInterface.backgroundColor = Color(0.0f, 0.0f, 0.0f, 1.0f);
    m_clear->createInput("renderPass") << m_shapeRenderPass->renderPass;

    // Rasterization stage for shape
    addStage(m_shapeRasterization.get());
    m_shapeRasterization->renderInterface.targetFBO << m_clear->fboOut;
    m_shapeRasterization->renderInterface.deviceViewport << renderInterface.deviceViewport;
    m_shapeRasterization->drawable << m_shapeRenderPass->renderPass;

    // Outputs
    renderInterface.rendered << m_shapeRasterization->renderInterface.rendered;
}

ColorGradientDemo::~ColorGradientDemo()
{
}
