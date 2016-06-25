
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Pipeline.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


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
class GLOPERATE_API DemoPipeline : public Pipeline
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        DemoPipeline, gloperate::Stage
      , "RenderStage"   // Tags
      , ""              // Icon
      , ""              // Annotations
      , "Demo pipeline displaying a rotating logo"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Interfaces
    RenderInterface<Pipeline> renderInterface; ///< Interface for rendering into a viewer


public:
    /**
    *  @brief
    *    Constructor
    *
    *  @param[in] environment
    *    Environment to which the stage belongs (must NOT be null!)
    *  @param[in] name
    *    Stage name
    */
    DemoPipeline(Environment * environment, const std::string & name = "DemoPipeline");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~DemoPipeline();


protected:
    // Stages
    MixerStage             * m_mixerStage;
    SplitStage             * m_splitStage;

    DemoTimerStage         * m_timerStage;
    DemoTriangleStage      * m_triangleStage;

    BasicFramebufferStage  * m_framebufferStage;
    TextureLoadStage       * m_textureLoadStage;
    ProceduralTextureStage * m_proceduralTextureStage;
};


} // namespace gloperate
