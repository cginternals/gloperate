
#pragma once

#include <gloperate-text/gloperate-text_api.h>


namespace gloperate_text
{

class FontFace;
class GlyphVertexCloud;
class Text;
class TextManager;

class TextLayouter
{
public:
    TextLayouter();
    virtual ~TextLayouter();

    void layout(Text & text, FontFace * font, GlyphVertexCloud & vertexCloud);
    void layout(TextManager & manager, FontFace * font, GlyphVertexCloud & vertexCloud);
};


} // namespace gloperate_text
