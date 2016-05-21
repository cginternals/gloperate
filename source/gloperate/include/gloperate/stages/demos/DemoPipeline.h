
#pragma once


#include <gloperate/pipeline/RenderPipeline.h>


namespace gloperate
{


class TextureLoadStage;
class MixerStage;


/**
*  @brief
*    Demo pipeline displaying a rotating logo
*/
class GLOPERATE_API DemoPipeline : public RenderPipeline
{
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
    TextureLoadStage * m_textureLoadStage;
    MixerStage       * m_mixerStage;
};


} // namespace gloperate
