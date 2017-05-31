
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


/**
*  @brief
*    Stage that creates a procedural texture
*/
class GLOPERATE_API ProceduralTextureStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        ProceduralTextureStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that creates a procedural texture"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Outputs
    Output<globjects::Texture *> texture;  ///< Texture object


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
    ProceduralTextureStage(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~ProceduralTextureStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess() override;

    // Helper functions
    void createTexture();


protected:
    // Data
    std::unique_ptr<globjects::Texture> m_texture; ///< Texture
};


} // namespace gloperate
