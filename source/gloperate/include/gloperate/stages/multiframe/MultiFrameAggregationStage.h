
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


class GLOPERATE_API MultiFrameAggregationStage : public Stage
{
public:
    CPPEXPOSE_DECLARE_COMPONENT(
        MultiFrameAggregationStage, gloperate::Stage
      , ""
      , ""
      , ""
      , "Stage that aggregates multiple subsequent frames into a single frame"
      , GLOPERATE_AUTHOR_ORGANIZATION
      , "v0.1.0"
    )


public:
    Input<globjects::Framebuffer *>  aggregationFBO;
    Input<globjects::Texture *>      texture;
    Input<bool>                      textureRerendered;
    Input<glm::vec4>                 viewport;
    Input<float>                     aggregationFactor;

    Output<globjects::Framebuffer *> aggregatedFBO;


public:
    MultiFrameAggregationStage(Environment * environment, const std::string & name = "MultiFrameAggregationStage");
    virtual ~MultiFrameAggregationStage();


protected:
    virtual void onContextInit(AbstractGLContext * context) override;
    virtual void onProcess(AbstractGLContext * context) override;

    void setupGeometry();
    void setupProgram();


protected:
    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer>      m_vertexBuffer;
    globjects::ref_ptr<globjects::Program>     m_program;
    globjects::ref_ptr<globjects::Shader>      m_vertexShader;
    globjects::ref_ptr<globjects::Shader>      m_fragmentShader;
};


} // namespace gloperate
