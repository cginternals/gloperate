
#pragma once


#include <glm/vec4.hpp>

#include <gloperate/pipeline/Stage.h>

#include <gloperate-text/GlyphVertexCloud.h>


namespace globjects
{
    class Framebuffer;
    class AbstractStringSource;
    class Shader;
    class Program;
}


namespace gloperate_text
{


class GlyphRenderer;
class GlyphVertexCloud;


class GLOPERATE_TEXT_API GlyphRenderStage : public gloperate::Stage
{
public:
    Input<GlyphVertexCloud *> vertexCloud;
    Input<glm::vec4> viewport;
    Input<globjects::Framebuffer *> targetFramebuffer;

    Output<bool> rendered;


public:
    explicit GlyphRenderStage(gloperate::Environment * environment, const std::string & name = "");
    virtual ~GlyphRenderStage();


protected:
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess() override;


protected:
    std::unique_ptr<globjects::AbstractStringSource> m_vSource;
    std::unique_ptr<globjects::AbstractStringSource> m_gSource;
    std::unique_ptr<globjects::AbstractStringSource> m_fSource;

    std::unique_ptr<globjects::Shader> m_vertexShader;
    std::unique_ptr<globjects::Shader> m_geometryShader;
    std::unique_ptr<globjects::Shader> m_fragmentShader;

    std::unique_ptr<globjects::Program> m_program;

    std::unique_ptr<GlyphRenderer> m_renderer;
};


} // namespace gloperate_text
