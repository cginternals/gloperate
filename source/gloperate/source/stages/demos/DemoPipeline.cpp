
#include <gloperate/stages/demos/DemoPipeline.h>

#include <gloperate/gloperate.h>
#include <gloperate/gloperate-version.h>
#include <gloperate/stages/base/TextureLoadStage.h>
#include <gloperate/stages/base/ProceduralTextureStage.h>
#include <gloperate/stages/base/BasicFramebufferStage.h>
#include <gloperate/stages/base/MixerStage.h>
#include <gloperate/stages/base/SplitStage.h>
#include <gloperate/stages/demos/DemoTimerStage.h>
#include <gloperate/stages/demos/DemoTriangleStage.h>


namespace gloperate
{


DemoPipeline::DemoPipeline(ViewerContext * viewerContext, const std::string & name, Pipeline * parent)
: RenderPipeline(viewerContext, name, parent)
, m_mixerStage(new MixerStage(viewerContext, "MixerStage", this))
, m_splitStage(new SplitStage(viewerContext, "SplitStage", this))
, m_timerStage(new DemoTimerStage(viewerContext, "TimerStage", this))
, m_triangleStage(new DemoTriangleStage(viewerContext, "TriangleStage", this))
, m_framebufferStage(new BasicFramebufferStage(viewerContext, "FramebufferStage", this))
, m_textureLoadStage(new TextureLoadStage(viewerContext, "TextureLoadStage", this))
, m_proceduralTextureStage(new ProceduralTextureStage(viewerContext, "ProceduralTextureStage", this))
{
    // Get data path
    std::string dataPath = gloperate::dataPath();
    if (dataPath.size() > 0) dataPath = dataPath + "/";
    else                     dataPath = "data/";

    // Register stages
    takeOwnership(m_textureLoadStage);
    takeOwnership(m_proceduralTextureStage);
    takeOwnership(m_framebufferStage);
    takeOwnership(m_timerStage);
    takeOwnership(m_triangleStage);
    takeOwnership(m_mixerStage);
    takeOwnership(m_splitStage);

    // Inputs
    m_timerStage->timeDelta << this->timeDelta;
    m_framebufferStage->viewport << this->deviceViewport;

    // Parameters
    *m_textureLoadStage->filename = dataPath + "gloperate/textures/gloperate-logo.png";

    // Triangle stage
    m_triangleStage->deviceViewport  << this->deviceViewport;
    m_triangleStage->targetFBO       << m_framebufferStage->fbo;
    m_triangleStage->colorTexture    << m_framebufferStage->colorTexture;
    m_triangleStage->backgroundColor << this->backgroundColor;
    m_triangleStage->texture         << m_textureLoadStage->texture;
    m_triangleStage->angle           << m_timerStage->virtualTime;

    // Split stage
    m_splitStage->viewport  << this->deviceViewport;
    m_splitStage->targetFBO << this->targetFBO;
    m_splitStage->texture1  << m_triangleStage->colorTextureOut;
    m_splitStage->texture2  << m_proceduralTextureStage->texture;

    // Outputs
    this->rendered << m_splitStage->rendered;

    /*
    // Get data path
    std::string dataPath = gloperate::dataPath();
    if (dataPath.size() > 0) dataPath = dataPath + "/";
    else                     dataPath = "data/";

    // Texture loader stage
    takeOwnership(m_textureLoadStage);
    *m_textureLoadStage->filename = dataPath + "gloperate/textures/gloperate-logo.png";

    // Procedural texture stage
    takeOwnership(m_proceduralTextureStage);

    // Framebuffer stage
    takeOwnership(m_framebufferStage);
    m_framebufferStage->viewport << this->deviceViewport;

    // Timer stage
    takeOwnership(m_timerStage);
    m_timerStage->timeDelta << this->timeDelta;

    // Triangle stage
    takeOwnership(m_triangleStage);
    m_triangleStage->deviceViewport  << this->deviceViewport;
    m_triangleStage->targetFBO       << m_framebufferStage->fbo;
    m_triangleStage->backgroundColor << this->backgroundColor;
    m_triangleStage->texture         << m_textureLoadStage->texture;
    m_triangleStage->angle           << m_timerStage->virtualTime;

    // Mixer stage
    takeOwnership(m_mixerStage);
//  m_mixerStage->viewport  << this->deviceViewport;
//  m_mixerStage->targetFBO << this->targetFBO;
//  m_mixerStage->texture   << m_framebufferStage->colorTexture;

    // Split stage
    takeOwnership(m_splitStage);
    m_splitStage->viewport  << this->deviceViewport;
    m_splitStage->targetFBO << this->targetFBO;
    m_splitStage->texture1  << m_framebufferStage->colorTexture;
    m_splitStage->texture2  << m_proceduralTextureStage->texture;

    // Outputs
    this->rendered << m_splitStage->rendered;
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
