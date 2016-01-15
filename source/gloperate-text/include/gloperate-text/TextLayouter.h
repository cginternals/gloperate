
#pragma once

#include <glm/fwd.hpp>

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{

class FontFace;
class GlyphVertex;
class GlyphVertexCloud;
class Text;
class TextManager;

class GLOPERATE_TEXT_API TextLayouter
{
public:
    TextLayouter();
    virtual ~TextLayouter();

    void layout(const Text & text, FontFace * font, GlyphVertexCloud & vertexCloud);
    void layout(const TextManager & manager, FontFace * font, GlyphVertexCloud & vertexCloud);

    void basicLayout(const Text & text, FontFace * font, GlyphVertex * startVertex, const glm::vec3 & start, const glm::vec3 & end);
};


} // namespace gloperate_text
