
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/pipeline/RenderPipeline.h>


namespace gloperate
{


class BasicFramebufferStage;
class TextureLoadStage;
class ProceduralTextureStage;
class MixerStage;
class SplitStage;
class DemoTimerStage;
class DemoTriangleStage;


/**
*  @brief
*    Demo pipeline displaying a rotating logo
*/
class GLOPERATE_API DemoPipeline : public RenderPipeline
{
    CPPEXPOSE_DECLARE_COMPONENT(DemoPipeline, gloperate::Stage)


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] viewerContext
    *    Viewer context to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    *  @param[in] parent
    *    Parent pipeline (can be null)
    */
    DemoPipeline(ViewerContext * viewerContext, const std::string & name = "DemoPipeline", Pipeline * parent = nullptr);

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoPipeline();


protected:
    // Stages
    TextureLoadStage       * m_textureLoadStage;
    ProceduralTextureStage * m_proceduralTextureStage;
    BasicFramebufferStage  * m_framebufferStage;
    DemoTimerStage         * m_timerStage;
    DemoTriangleStage      * m_triangleStage;
    MixerStage             * m_mixerStage;
    SplitStage             * m_splitStage;
};


} // namespace gloperate
