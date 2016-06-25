
#pragma once


#include <string>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/base/ref_ptr.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Parameter.h>
#include <gloperate/pipeline/Output.h>


namespace gloperate
{


/**
*  @brief
*    Stage that loads a texture from a file
*/
class GLOPERATE_API TextureLoadStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        TextureLoadStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that loads a texture from a file"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Parameters
    Parameter<std::string>       filename; ///< Texture filename

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
    TextureLoadStage(Environment * environment, const std::string & name = "TextureLoadStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~TextureLoadStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void loadTexture();


protected:
    // Data
    globjects::ref_ptr<globjects::Texture> m_texture; ///< Texture
};


} // namespace gloperate
