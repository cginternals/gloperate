
#include <gloperate-text/stages/FontImporterStage.h>

#include <gloperate/base/ResourceManager.h>

#include <gloperate-text/FontLoader.h>
#include <gloperate-text/FontFace.h>


namespace gloperate_text
{


FontImporterStage::FontImporterStage(gloperate::Environment * environment, const std::string & name)
: Stage(environment, name)
, resourceManager("resourceManager", this)
, fontFilePath("fontFilePath", this)
, font("font", this)
{
}

FontImporterStage::~FontImporterStage()
{
}

void FontImporterStage::onContextInit(gloperate::AbstractGLContext * /*context*/)
{
    m_importer.reset(new gloperate_text::FontLoader(*(*resourceManager)));
}

void FontImporterStage::onProcess(gloperate::AbstractGLContext * context)
{
    FontFace * newFont = m_importer->load((*fontFilePath).path());

    if (newFont == nullptr)
    {
        return;
    }

    m_font = newFont;
    (*font) = /*ToDo: use setValue?*/ m_font;

    // invalidateOutputs(); // TODO: check if outputs are set to valid
}


} // namespace gloperate_text
