
#include <gloperate-text/stages/FontImporterStage.h>

#include <gloperate/base/Environment.h>
#include <gloperate/base/ResourceManager.h>

#include <gloperate-text/FontLoader.h>
#include <gloperate-text/FontFace.h>


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


void FontImporterStage::onProcess(gloperate::AbstractGLContext *)
{
    auto importer = FontLoader{ m_environment->resourceManager() };
    auto newFont = std::unique_ptr<FontFace>{ importer.load(fontFilePath->path()) };

    if (newFont != nullptr)
    {
        m_font = std::move(newFont);
        font.setValue(m_font.get());
    }
}


} // namespace gloperate_text
