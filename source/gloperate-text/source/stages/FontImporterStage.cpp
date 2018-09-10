
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

void FontImporterStage::onProcess()
{
    auto newFont = std::unique_ptr<openll::FontFace>{ m_environment->resourceManager()->load<openll::FontFace>(fontFilePath.value().path())};

    if (newFont)
    {
        m_font = std::move(newFont);
        font.setValue(m_font.get());
    }
}


} // namespace gloperate_text
