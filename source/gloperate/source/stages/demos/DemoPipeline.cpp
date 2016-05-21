
#include <gloperate/stages/demos/DemoPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/gloperate-version.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/MixerStage.h>


namespace gloperate
{


DemoPipeline::DemoPipeline(ViewerContext * viewerContext, const std::string & name, Pipeline * parent)
: RenderPipeline(viewerContext, name, parent)
, m_textureLoadStage(new TextureLoadStage(viewerContext, "TextureLoadStage", this))
, m_mixerStage(new MixerStage(viewerContext, "MixerStage", this))
{
    // Get data path
    std::string dataPath = gloperate::dataPath();
    if (dataPath.size() > 0) dataPath = dataPath + "/";
    else                     dataPath = "data/";

    // Texture loader stage
    takeOwnership(m_textureLoadStage);
    *m_textureLoadStage->filename = dataPath + "gloperate/textures/gloperate-logo.png";

    // Mixer stage
    takeOwnership(m_mixerStage);
    m_mixerStage->viewport  << this->deviceViewport;
    m_mixerStage->targetFBO << this->targetFBO;
    m_mixerStage->texture   << m_textureLoadStage->texture;
}

DemoPipeline::~DemoPipeline()
{
}


CPPEXPOSE_COMPONENT(
    DemoPipeline, gloperate::Stage
  , "RenderStage"   // Tags
  , ""              // Icon
  , ""              // Annotations
  , "Demo pipeline displaying a rotating logo"
  , GLOPERATE_AUTHOR_ORGANIZATION
  , "v1.0.0"
)


} // namespace gloperate
