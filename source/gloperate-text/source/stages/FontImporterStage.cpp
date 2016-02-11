
#include <gloperate-text/stages/FontImporterStage.h>

#include <gloperate/resources/ResourceManager.h>

#include <gloperate-text/FontLoader.h>
#include <gloperate-text/FontFace.h>


namespace gloperate_text
{


FontImporterStage::FontImporterStage()
: AbstractStage("FontImporterStage")
{
    addInput("resourceManager", resourceManager);
    addInput("fontFilePath", fontFilePath);

    addOutput("font", font);
}

FontImporterStage::~FontImporterStage()
{
}

void FontImporterStage::initialize()
{
    m_importer.reset(new gloperate_text::FontLoader(*resourceManager.data()));
}

void FontImporterStage::process()
{
    FontFace * newFont = m_importer->load(fontFilePath.data().toString());

    if (newFont == nullptr)
    {
        return;
    }

    m_font = newFont;
    font.data() = m_font;

    invalidateOutputs();
}


} // namespace gloperate_text
