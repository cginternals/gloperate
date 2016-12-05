
#pragma once


#include <cppexpose/plugin/plugin_api.h>

#include <glm/vec4.hpp>

#include <globjects/base/ref_ptr.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <globjects/Program.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>
#include <globjects/Framebuffer.h>

#include <gloperate/gloperate-version.h>
#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>


namespace gloperate
{


/**
*  @brief
*    Stage that aggregates multiple subsequent frames into a single framebuffer
*/
class GLOPERATE_API MultiFrameAggregationStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        MultiFrameAggregationStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that aggregates multiple subsequent frames into a single framebuffer"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    // Inputs
    Input<globjects::Framebuffer *>  aggregationFBO;    ///< FBO to aggregate into
    Input<globjects::Texture *>      texture;           ///< New frame to add to aggregation
    Input<bool>                      textureRerendered; ///< Add texture to aggregation?
    Input<glm::vec4>                 viewport;          ///< Target viewport
    Input<float>                     aggregationFactor; ///< Weight of new frame in current aggregation

    // Outputs
    Output<globjects::Framebuffer *> aggregatedFBO;     ///< Framebuffer containing aggregation


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
    MultiFrameAggregationStage(Environment * environment, const std::string & name = "MultiFrameAggregationStage");

    /**
    *  @brief
    *    Destructor
    */
    virtual ~MultiFrameAggregationStage();


protected:
    // Virtual Stage interface
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    // Helper functions
    void setupGeometry();
    void setupProgram();


protected:
    // Data
    globjects::ref_ptr<globjects::VertexArray> m_vao;            ///< VAO for screen aligned quad
    globjects::ref_ptr<globjects::Buffer>      m_vertexBuffer;   ///< VBO for screen aligned quad
    globjects::ref_ptr<globjects::Program>     m_program;        ///< Shader program used for aggregation
    globjects::ref_ptr<globjects::Shader>      m_vertexShader;   ///< Vertex shader
    globjects::ref_ptr<globjects::Shader>      m_fragmentShader; ///< Fragment shader
};


} // namespace gloperate
