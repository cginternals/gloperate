
#include <gloperate-text/stages/FontImporterStage.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ResourceManager.h>

#include <openll/FontFace.h>


namespace gloperate_text
{


FontImporterStage::FontImporterStage(gloperate::Environment * environment, const std::string & name)
: Stage{ environment, "FontImporterStage", name }
, fontFilePath{ "fontFilePath", this }
, font{ "font", this }
{
}

FontImporterStage::~FontImporterStage()
{
}

void FontImporterStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    font.invalidate();
}

void FontImporterStage::onContextDeinit(gloperate::AbstractGLContext * /*context*/)
{
    m_font = nullptr;

    font.setValue(nullptr);
}

void FontImporterStage::onProcess()
{
    cppassist::debug(0, "gloperate-text") << "Attempt to load font from " << fontFilePath.value().path();

    auto newFont = std::unique_ptr<openll::FontFace>{ m_environment->resourceManager()->load<openll::FontFace>(fontFilePath.value().path())};

    if (newFont)
    {
        cppassist::debug(0, "gloperate-text") << "Loaded font " << newFont.get() << " from " << fontFilePath.value().path();

        m_font = std::move(newFont);
        font.setValue(m_font.get());
    }
}


} // namespace gloperate_text
