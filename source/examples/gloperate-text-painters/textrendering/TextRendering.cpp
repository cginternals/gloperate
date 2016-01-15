
#include "TextRendering.h"

#include <cpplocate/ModuleInfo.h>

#include <iozeug/FilePath.h>

#include <gloperate/painter/ViewportCapability.h>
#include <gloperate/painter/TargetFramebufferCapability.h>

#include <gloperate-text/FontFace.h>
#include <gloperate-text/FontImporter.h>
#include <gloperate-text/GlyphRenderer.h>
#include <gloperate-text/TextLayouter.h>
#include <gloperate-text/Text.h>
#include <gloperate-text/TextConverter.h>
#include <gloperate-text/Encoding.h>

#include <glbinding/gl/gl.h>


using namespace gloperate;
using namespace globjects;
using namespace gl;


TextRendering::TextRendering(ResourceManager & resourceManager, const cpplocate::ModuleInfo & moduleInfo)
: Painter("TextRendering", resourceManager, moduleInfo)
, m_resourceManager(resourceManager)
{
    // Get data path
    m_dataPath = moduleInfo.value("dataPath");
    m_dataPath = iozeug::FilePath(m_dataPath).path();
    if (m_dataPath.size() > 0) m_dataPath = m_dataPath + "/";
    else                       m_dataPath = "data/";

    // Setup painter
    m_targetFramebufferCapability = addCapability(new gloperate::TargetFramebufferCapability());
    m_viewportCapability = addCapability(new gloperate::ViewportCapability());
}

TextRendering::~TextRendering()
{
}

void TextRendering::onInitialize()
{
    gloperate_text::FontImporter importer(m_resourceManager);

    m_fontFace = importer.loadFont(m_dataPath + "gloperate-text/fonts/test.txt");

    m_renderer = new gloperate_text::GlyphRenderer;

    gloperate_text::TextLayouter layouter;

    std::u32string string;
    gloperate_text::TextConverter::encode(std::string("Hallo"), gloperate_text::Encoding::ASCII, string);

    gloperate_text::Text text(string);
    layouter.layout(text, m_fontFace, m_vertexCloud);
}

void TextRendering::onPaint()
{
    if (m_viewportCapability->hasChanged())
    {
        glViewport(m_viewportCapability->x(), m_viewportCapability->y(), m_viewportCapability->width(), m_viewportCapability->height());

        m_viewportCapability->setChanged(false);
    }

    globjects::Framebuffer * fbo = m_targetFramebufferCapability->framebuffer();

    if (!fbo)
    {
        fbo = globjects::Framebuffer::defaultFBO();
    }

    fbo->bind(GL_FRAMEBUFFER);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_renderer->render(m_vertexCloud);

    globjects::Framebuffer::unbind(GL_FRAMEBUFFER);
}
