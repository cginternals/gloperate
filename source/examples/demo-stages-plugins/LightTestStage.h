
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace globjects
{
    class Texture;
}


class LightTestStage : public gloperate::Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        LightTestStage, gloperate::Stage
      , "" // Tags
      , "" // Icon
      , "" // Annotations
      , "Stage that renders a rotating cube using provided lighting information"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Interfaces
    gloperate::RenderInterface renderInterface;                  ///< Interface for rendering into a viewer

    // Inputs
    Input<float> glossiness;                          ///< Glossiness of the cube (0.0 to 1.0)
    Input<float> totalTime;                           ///< Total running time
    Input<globjects::Texture *> lightColorTypeData;   ///< Buffer Texture with color & type information
    Input<globjects::Texture *> lightPositionData;    ///< Buffer Texture with position information
    Input<globjects::Texture *> lightAttenuationData; ///< Buffer Texture with attenuation information


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
    LightTestStage(gloperate::Environment * environment, const std::string & name = "LightTestStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~LightTestStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess();


protected:
    // Rendering objects
    std::unique_ptr<globjects::VertexArray> m_vao;
    std::unique_ptr<globjects::Buffer>      m_vertexBuffer;
    std::unique_ptr<globjects::Program>     m_program;
    std::unique_ptr<globjects::Shader>      m_vertexShader;
    std::unique_ptr<globjects::Shader>      m_fragmentShader;
};
