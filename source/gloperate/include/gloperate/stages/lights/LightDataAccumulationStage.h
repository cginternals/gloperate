
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


struct LightDefinition;


class GLOPERATE_API LightDataAccumulationStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        LightDataAccumulationStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that takes any amount of lights as seperate inputs and creates texture buffers containing information of all lights"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )

public:
    Output<globjects::Texture *> colorTypeData;
    Output<globjects::Texture *> positionData;
    Output<globjects::Texture *> attenuationData;

public:
    LightDataAccumulationStage(Environment * environment, const std::string & name = "LightDataAccumulationStage");

    virtual ~LightDataAccumulationStage();

    Input<LightDefinition> * createLightInput();

protected:
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    void setupBufferTextures();

protected:
    globjects::Texture * m_colorTypeTexture;
    globjects::Texture * m_positionTexture;
    globjects::Texture * m_attenuationTexture;

    globjects::Buffer * m_colorTypeBuffer;
    globjects::Buffer * m_positionBuffer;
    globjects::Buffer * m_attenuationBuffer;

    std::vector< Input<LightDefinition> * > m_lightInputs;
};


} // namespace gloperate
