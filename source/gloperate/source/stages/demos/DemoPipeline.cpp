
#include <gloperate/stages/demos/DemoPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/gloperate-version.h>
#include <gloperate/stages/base/ProceduralTextureStage.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/MixerStage.h>
#include <gloperate/stages/demos/DemoTimerStage.h>
#include <gloperate/stages/demos/DemoTriangleStage.h>


namespace gloperate
{


DemoPipeline::DemoPipeline(ViewerContext * viewerContext, const std::string & name, Pipeline * parent)
: RenderPipeline(viewerContext, name, parent)
, m_textureLoadStage(new TextureLoadStage(viewerContext, "TextureLoadStage", this))
, m_proceduralTextureStage(new ProceduralTextureStage(viewerContext, "ProceduralTextureStage", this))
, m_timerStage(new DemoTimerStage(viewerContext, "TimerStage", this))
, m_triangleStage(new DemoTriangleStage(viewerContext, "TriangleStage", this))
//, m_mixerStage(new MixerStage(viewerContext, "MixerStage", this))
, m_mixerStage(nullptr)
{
    // Get data path
    std::string dataPath = gloperate::dataPath();
    if (dataPath.size() > 0) dataPath = dataPath + "/";
    else                     dataPath = "data/";

    // Procedural texture stage
    takeOwnership(m_proceduralTextureStage);

    // Texture loader stage
    takeOwnership(m_textureLoadStage);
    *m_textureLoadStage->filename = dataPath + "gloperate/textures/gloperate-logo.png";

    // Timer stage
    takeOwnership(m_timerStage);
    m_timerStage->timeDelta << this->timeDelta;

    // Triangle stage
    takeOwnership(m_triangleStage);
    m_triangleStage->deviceViewport  << this->deviceViewport;
    m_triangleStage->targetFBO       << this->targetFBO;
    m_triangleStage->backgroundColor << this->backgroundColor;
    m_triangleStage->texture         << m_textureLoadStage->texture;
    m_triangleStage->angle           << m_timerStage->virtualTime;
    this->rendered                   << m_triangleStage->rendered;

    /*
    // Mixer stage
    takeOwnership(m_mixerStage);
    m_mixerStage->viewport  << this->deviceViewport;
    m_mixerStage->targetFBO << this->targetFBO;
    m_mixerStage->texture   << m_textureLoadStage->texture;
    */
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
