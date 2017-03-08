
#pragma once


#include <glm/glm.hpp>

#include <cppexpose/plugin/plugin_api.h>

#include <globjects/VertexArray.h>
#include <globjects/Program.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/base/ExtendedProperties.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/pipeline/Input.h>
#include <gloperate/pipeline/Output.h>


namespace globjects {
    class Texture;
    class Buffer;
    class Framebuffer;
}


namespace gloperate
{


/**
*  @brief
*    Stage that renders and mixes textures into a full-screen quad
*/
class GLOPERATE_API MixerStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        MixerStage, gloperate::Stage
      , ""   // Tags
      , ""   // Icon
      , ""   // Annotations
      , "Stage that renders and mixes textures into a full-screen quad"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v1.0.0"
    )


public:
    // Inputs
    Input<glm::vec4>                 viewport;       ///< Viewport
    Input<globjects::Framebuffer *>  targetFBO;      ///< Target FBO into which to render
    Input<globjects::Texture *>      texture;        ///< Color texture
    Input<std::string>               vertexShader;   ///< Vertex shader filename
    Input<std::string>               geometryShader; ///< Geometry shader filename
    Input<std::string>               fragmentShader; ///< Fragment shader filename

    // Outputs
    Output<bool>                     rendered;  ///< 'true' if output has been rendered
    Output<globjects::Framebuffer *> fboOut;    ///< Pass through of target FBO


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
    MixerStage(Environment * environment, const std::string & name = "");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~MixerStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onContextDeinit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;
    virtual void onInputValueChanged(AbstractSlot * slot) override;

    // Helper functions
    void buildGeometry();
    void buildProgram();


protected:
    std::unique_ptr<globjects::VertexArray> m_vao;            ///< Screen-aligned quad geometry
    std::unique_ptr<globjects::Program>     m_program;        ///< Shader program
    bool                                    m_rebuildProgram; ///< 'true', if program needs rebuild
    std::unique_ptr<globjects::Buffer>      m_buffer;

    std::unique_ptr<globjects::Shader>  m_vertexShader;
    std::unique_ptr<globjects::Shader>  m_geometryShader;
    std::unique_ptr<globjects::Shader>  m_fragmentShader;
};


} // namespace gloperate
