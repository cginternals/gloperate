
#pragma once

#include <cppexpose/plugin/plugin_api.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Output.h>


namespace globjects
{

class Texture;
class Buffer;

} // namespace globjects


namespace gloperate
{


struct Light;


/**
*  @brief
*    Stage that takes Light objects as inputs and creates texture buffers containing information of all lights
*/
class GLOPERATE_API LightBufferTextureStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        LightBufferTextureStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that takes Light objects as inputs and creates texture buffers containing information of all lights"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )

public:
    // Outputs
    Output<globjects::Texture *> colorTypeData;   ///< Buffer texture containing color (RGB channels) and type (alpha channel) information of lights
    Output<globjects::Texture *> positionData;    ///< Buffer texture containing position information of lights
    Output<globjects::Texture *> attenuationData; ///< Buffer texture containing attenuation information of lights

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
    LightBufferTextureStage(Environment * environment, const std::string & name = "LightBufferTextureStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~LightBufferTextureStage();

    /**
    *  @brief
    *    Create additional Light input
    *
    *  @return
    *    Pointer to new input
    */
    Input<Light> * createLightInput();

protected:
    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void setupBufferTextures();

protected:
    std::unique_ptr<globjects::Texture> m_colorTypeTexture;     ///< Buffer texture for color & type information
    std::unique_ptr<globjects::Texture> m_positionTexture;      ///< Buffer texture for position information
    std::unique_ptr<globjects::Texture> m_attenuationTexture;   ///< Buffer texture for attenuation information

    std::unique_ptr<globjects::Buffer>  m_colorTypeBuffer;       ///< Buffer for color & type information
    std::unique_ptr<globjects::Buffer>  m_positionBuffer;        ///< Buffer for position information
    std::unique_ptr<globjects::Buffer>  m_attenuationBuffer;     ///< Buffer for attenuation information

    std::vector< Input<Light> * > m_lightInputs; ///< Light inputs
};


} // namespace gloperate
