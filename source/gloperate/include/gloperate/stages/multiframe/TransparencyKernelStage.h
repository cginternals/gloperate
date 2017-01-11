#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/pipeline/Input.h>


namespace globjects
{
    class Texture;
}


namespace gloperate
{


/**
*  @brief
*    Stage that provides noise for random discard based transparency
*/
class GLOPERATE_API TransparencyKernelStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        TransparencyKernelStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that provides noise for random discard based transparency"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<bool> reprocess; ///< Recreate texture

    // Outputs
    Output<globjects::Texture *> transparencyMaskTexture; ///< Transparency kernel


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
    TransparencyKernelStage(gloperate::Environment * environment, const std::string & name = "TransparencyKernelStage");


protected:
    // Virtual Stage interface
    virtual void onProcess(gloperate::AbstractGLContext * context) override;
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;

    // Helper Functions
    void regenerateKernel();


protected:
    globjects::ref_ptr<globjects::Texture> m_texture;
};


} // namespace gloperate
