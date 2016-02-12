
#include "TextRenderingPainter.h"

#include <glm/gtc/constants.hpp>
#include <glm/vec2.hpp>

#include <reflectionzeug/property/extensions/GlmProperties.h>

#include <gloperate/painter/ViewportCapability.h>
//#include <gloperate/painter/VirtualTimeCapability.h>
#include <gloperate/painter/TargetFramebufferCapability.h>
#include <gloperate/painter/TypedRenderTargetCapability.h>

#include <gloperate-text/Alignment.h>
#include <gloperate-text/LineAnchor.h>

#include <QtWidgets/qapplication.h>
#include <QtGui/qscreen.h>


TextRenderingPainter::TextRenderingPainter(gloperate::ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo)
: PipelinePainter("TextRendering", resourceManager, moduleInfo, m_pipeline)
{
    auto targetFBO = addCapability(new gloperate::TargetFramebufferCapability());
    auto viewport = addCapability(new gloperate::ViewportCapability());
    //auto time = addCapability(new gloperate::VirtualTimeCapability());

    m_pipeline.targetFBO.setData(targetFBO);
    m_pipeline.viewport.setData(viewport);
    //m_pipeline.time.setData(time);
    m_pipeline.resourceManager.setData(&resourceManager);

    targetFBO->changed.connect([this]() { m_pipeline.targetFBO.invalidate(); });
    viewport->changed.connect([this]() { m_pipeline.viewport.invalidate(); });
    //time->changed.connect([this]() { m_pipeline.time.invalidate(); });
    //time->setLoopDuration(glm::pi<float>() * 2);

    addProperty(createProperty("GlyphSequence", m_pipeline.string));

    addProperty(createProperty("Filename", m_pipeline.fontFilename));
    addProperty(createProperty("Size", m_pipeline.fontSize));
    addProperty(createProperty("NumChars", m_pipeline.numChars));
    addProperty(createProperty("PixelPerInch", m_pipeline.pixelPerInch));
    addProperty(createProperty("Origin", m_pipeline.origin));
    addProperty(createProperty("Margins", m_pipeline.margins));
    addProperty(createProperty("WordWrap", m_pipeline.wordWrap));
    addProperty(createProperty("LineWidth", m_pipeline.lineWidth));
    addProperty(createProperty("Alignment", m_pipeline.alignment));
    addProperty(createProperty("LineAnchor", m_pipeline.lineAnchor));
    addProperty(createProperty("L1-Cache-Optimized", m_pipeline.optimized));

    auto screen = QApplication::primaryScreen();
    assert(screen);
    m_pipeline.pixelPerInch = screen->logicalDotsPerInch();
    m_pipeline.pixelPerInch = screen->physicalDotsPerInch();
}
