
#include "TextRenderingPainter.h"

#include <glm/gtc/constants.hpp>

#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/VirtualTimeCapability.h>
#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/CameraCapability.h>
#include <gloperate/painter/PerspectiveProjectionCapability.h>
#include <gloperate/painter/TypedRenderTargetCapability.h>
#include <gloperate/base/ExplicitEquidistantColorGradient.h>

#include <gloperate-text/Encoding.h>

#include "TextGenerationAlgorithm.h"


TextRenderingPainter::TextRenderingPainter(gloperate::ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo)
: PipelinePainter("TextRendering", resourceManager, moduleInfo, m_pipeline)
{
    // Setup painter
    auto targetFBO = addCapability(new gloperate::TargetFramebufferCapability());
    auto viewport = addCapability(new gloperate::ViewportCapability());
    auto time = addCapability(new gloperate::VirtualTimeCapability());

    m_pipeline.targetFBO.setData(targetFBO);
    m_pipeline.viewport.setData(viewport);
    m_pipeline.time.setData(time);
    m_pipeline.resourceManager.setData(&resourceManager);

    targetFBO->changed.connect([this]() { m_pipeline.targetFBO.invalidate(); });
    viewport->changed.connect([this]() { m_pipeline.viewport.invalidate(); });
    time->changed.connect([this]() { m_pipeline.time.invalidate(); });

    time->setLoopDuration(glm::pi<float>() * 2);

    reflectionzeug::PropertyGroup * fontLoading = addGroup("Font");
    fontLoading->addProperty(createProperty("Filename", m_pipeline.fontFilename));

    reflectionzeug::PropertyGroup * textGeneration = addGroup("Text");
    textGeneration->addProperty(createProperty("Algorithm", m_pipeline.textGenerationAlgorithm));
    textGeneration->addProperty(createProperty("StaticText", m_pipeline.staticText));
    textGeneration->addProperty(createProperty("Length", m_pipeline.length));
    //textGeneration->addProperty(createProperty("Encoding", m_pipeline.encoding));
    textGeneration->addProperty(createProperty("XRepeat", m_pipeline.xRepeat));
    textGeneration->addProperty(createProperty("YRepeat", m_pipeline.yRepeat));
    textGeneration->addProperty(createProperty("JiggleRadius", m_pipeline.jiggleRadius));

    reflectionzeug::PropertyGroup * rendering = addGroup("Rendering");
    rendering->addProperty(createProperty("FontColor", m_pipeline.fontColor));
    rendering->addProperty(createProperty("DistanceThreshold", m_pipeline.distanceThreshold));
}
