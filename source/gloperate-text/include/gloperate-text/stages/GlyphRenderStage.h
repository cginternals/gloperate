
#pragma once


#include <glm/vec4.hpp>

#include <gloperate/pipeline/Stage.h>
#include <gloperate/stages/interfaces/RenderInterface.h>

#include <gloperate-text/gloperate-text_api.h>

#include <openll/GlyphVertexCloud.h>


namespace globjects
{
    class Framebuffer;
    class AbstractStringSource;
    class Shader;
    class Program;
}


namespace openll
{


class GlyphRenderer;
class GlyphVertexCloud;


} // namespace


namespace gloperate_text
{


class GLOPERATE_TEXT_API GlyphRenderStage : public gloperate::Stage
{
public:
    gloperate::RenderInterface renderInterface; ///< Interface to render into render targets

    Input<openll::GlyphVertexCloud *> vertexCloud;


public:
    explicit GlyphRenderStage(gloperate::Environment * environment, const std::string & name = "");
    virtual ~GlyphRenderStage();


protected:
    virtual void onContextInit(gloperate::AbstractGLContext * context) override;
    virtual void onContextDeinit(gloperate::AbstractGLContext * context) override;
    virtual void onProcess() override;


protected:
    std::unique_ptr<openll::GlyphRenderer> m_renderer;
};


} // namespace gloperate_text
