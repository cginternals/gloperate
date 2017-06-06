
#pragma once


#include <memory>

#include <glm/fwd.hpp>

#include <gloperate-text/gloperate-text_api.h>


namespace globjects
{
    class Program;
    class AbstractStringSource;
}


namespace gloperate_text
{


class GlyphVertexCloud;


class GLOPERATE_TEXT_API GlyphRenderer
{
public:
    explicit GlyphRenderer(globjects::Program * program);
    virtual ~GlyphRenderer();

    static std::unique_ptr<globjects::AbstractStringSource> vertexShaderSource();
    static std::unique_ptr<globjects::AbstractStringSource> geometryShaderSource();
    static std::unique_ptr<globjects::AbstractStringSource> fragmentShaderSource();

    GlyphRenderer & operator=(const GlyphRenderer &) = delete;

    globjects::Program * program();
    const globjects::Program * program() const;

    void render(const GlyphVertexCloud & vertexCloud) const;
    void renderInWorld(const GlyphVertexCloud & vertexCloud, const glm::mat4 & viewProjectionMatrix) const;


protected:
    globjects::Program * m_program;  ///< Program used for rendering (owned by stage)
};


} // namespace gloperate_text
